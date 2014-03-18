<?

if (!$locale_user_is_translator) {
	exit('not authorized');
}

$locale_name = get_locale_name();

if (isset($_POST['command'])) {
	$command = $_POST['command'];

	switch ($command) {
	case 'commit':
		require_once 'git_funcs.inc';
		git_add_all();
		run_xhr(
			'git commit -m '.
			escapeshellarg('Update '.$locale_name.' translation').' '.
			'--author '.escapeshellarg($db_user['name'].' <'.$db_user['email'].'>')
		);
		break;

	case 'push':
		run_xhr('git push downstream '.$locale);
		break;

	case 'checkpull':
		$output = run_github('GET', 'repos/'.$repo.'/pulls/'.$db_locale['pullrequest']);
		$output = implode("\n", $output);
		$json = json_decode($output, true);
		if ($json['state'] != 'open') {
			Translations::SetPullRequest($repo, $locale, 0);
		}
		break;

	case 'pull':
		if ($request = $db_locale['pullrequest']) {
			run_github('PATCH', 'repos/'.$repo.'/pulls/'.$request, array(
				'title' => 'Update '.$locale_name.' translation',
				'body' => '',
				'state' => 'open'
			));
		} else {
			$output = run_github('POST', 'repos/'.$repo.'/pulls', array(
				'title' => 'Update '.$locale_name.' translation',
				'body' => '',
				'head' => $github_user.':'.$locale,
				'base' => $db_repo['branch']
			));
			$output = implode("\n", $output);
			$json = json_decode($output, true);
			$number = $json['number'];

			Translations::SetPullRequest($repo, $locale, $number);
		}
		Log::InsertLogItem($db_user['id'], 'locale_pullrequest', $repo, $locale);
		break;
	}
	exit;
}

$commands[] = array(
	'string' => 'Saving your changes',
	'name' => 'commit'
);
$commands[] = array(
	'string' => 'Updating our fork',
	'name' => 'push'
);
if ($db_locale['pullrequest']) {
	$commands[] = array(
		'string' => 'Checking the status of our last pull request',
		'name' => 'checkpull'
	);
}
$commands[] = array(
	'string' => 'Creating/updating pull request',
	'name' => 'pull'
);
$command_redirect = 'locale.php';

require_once 'twig.inc';
render('repo/locale/git.twig', compact('command_redirect'));
