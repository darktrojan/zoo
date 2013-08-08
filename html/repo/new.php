<?

if (!isset($_POST['locale'])) {
	exit('not enough args');
}

if (!isset($db_user)) {
	exit('need a valid user');
}

$locale = $_POST['locale'];
if (!preg_match('/^[a-z]{2}(-[A-Z]{2})?$/', $locale)) {
	exit('not a valid locale');
}

$locale_workdir = $repo_path.'/'.$locale;
$locale_path_abs = $locale_workdir.'/'.$repo_locale_path;
$locale_xpi_path_abs = $locale_workdir;
if ($db_repo['xpi_path']) {
	$locale_xpi_path_abs .= '/'.$db_repo['xpi_path'];
}

function ensure_dir($file, $limit) {
	$dir = dirname($file);
	if ($dir == $limit) {
		return;
	}
	if (!is_dir($dir)) {
		ensure_dir($dir, $limit);
		echo 'making '.$dir."\n";
		mkdir($dir);
	}
}

if (isset($_POST['command'])) {
	$command = $_POST['command'];

	switch ($command) {
	case 'workdir':
		run_xhr($script_root.'/git-new-workdir '.escapeshellarg($repo_path.'/master').' '.escapeshellarg($locale_workdir));
		chdir($locale_workdir);
		run_xhr('git checkout -b '.escapeshellarg($locale));
		break;

	case 'copy':
		chdir($locale_workdir);
		if ($db_repo['jetpack']) {
			$src = $repo_locale_path.'/en-US.properties';
			$dest = $repo_locale_path.'/'.$locale.'.properties';

			if (!file_exists($dest)) {
				copy($src, $dest);
				run_xhr('git add '.escapeshellarg($dest));
			}

		} else if (!is_dir($locale_path_abs.'/'.$locale)) {
			foreach ($db_files as $file) {
				$dest = $locale_path_abs.'/'.$locale.'/'.$file['file'];
				ensure_dir($dest, $locale_path_abs);
				copy($locale_path_abs.'/en-US/'.$file['file'], $dest);
				run_xhr('git add '.escapeshellarg($repo_locale_path.'/'.$locale.'/'.$file['file']));
			}
			$cm = new ChromeManifest($locale_xpi_path_abs.'/chrome.manifest');
			$cm->add_locale($locale);
			$cm->save();
			chdir($db_repo['xpi_path']);
			run_xhr('git add chrome.manifest');
		}
		break;

	case 'db':
		$locales = $db_repo['locales'] ? explode(',', $db_repo['locales']) : array();
		$locales[] = $locale;

		DBConnection::BeginTransaction();
		Repos::UpdateRepoLocales($repo, implode(',', array_unique($locales)));
		Translations::InsertTranslation($repo, $locale, $db_user['id']);
		Files::CreateLocale($repo, $locale);
		Log::InsertLogItem($db_user['id'], 'locale_new', $repo, $locale);
		DBConnection::CommitTransaction();
		break;
	}
	exit;
}

if (is_dir($locale_workdir)) {
	header('Location: /'.$repo.'/'.$locale.'/locale.php');
	exit;
}

$commands[] = array(
	'string' => 'Creating a new workdir',
	'name' => 'workdir',
	'params' => 'locale='.$locale
);
$commands[] = array(
	'string' => 'Copying locale files',
	'name' => 'copy',
	'params' => 'locale='.$locale
);
$commands[] = array(
	'string' => 'Updating the database',
	'name' => 'db',
	'params' => 'locale='.$locale
);
$command_redirect = $locale.'/locale.php';

require_once 'twig.inc';
render('repo/new.twig', compact('command_redirect'));
