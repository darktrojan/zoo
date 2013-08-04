<?

function create_repo($repo, $update = false, $branch = null) {
	global $repo_root, $db_user;

	$repo_path = $repo_root.'/'.$repo;
	$pattern = '/locale\s+\w+\s+([a-z]{2}(-[A-Z]{2})?)\s+(.*)$/';

	$old_dir = getcwd();

	chdir($repo_path.'/master');
	if ($branch) {
		run('git checkout '.escapeshellarg($branch));
		run('git merge --ff-only '.escapeshellarg('origin/'.$branch));
	}
	$manifest = run('find -name "chrome.manifest"');

	// TODO something useful here
	if (sizeof($manifest) != 1) {
		exit('need exactly 1 chrome.manifest, '.sizeof($manifest).' found');
	}
	$manifest = substr($manifest[0], 2);
	if ($manifest != 'chrome.manifest') {
		$xpi_path = dirname($manifest);
		chdir($xpi_path);
	}

	$rdf = new InstallRDF('install.rdf');
	$name = $rdf->get_name();

	$cm = new ChromeManifest('chrome.manifest');
	$repo_locale_path = $cm->locale_path;

	if (!isset($repo_locale_path)) {
		exit('en-US translation not found');
	}

	$locales = $cm->locales;
	foreach (Translations::SelectTranslationsByRepo($repo) as $t) {
		$locales[] = $t['locale'];
	}
	$locales = array_unique($locales);

	DBConnection::BeginTransaction();
	if ($update) {
		Repos::UpdateRepo(
			$repo, $name, implode(',', $locales), $xpi_path, $repo_locale_path, $branch
		);
		Log::InsertLogItem($db_user['id'], 'repo_update', $repo, '');
	} else {
		Repos::InsertRepo(
			$db_user['id'], $repo, $name, implode(',', $locales), $xpi_path, $repo_locale_path, $branch
		);
		Log::InsertLogItem($db_user['id'], 'repo_new', $repo, '');
	}

	$locale_dir = $repo_path.'/master/'.$repo_locale_path.'/en-US';
	chdir($locale_dir);
	foreach (run('find -name "*.dtd" -o -name "*.properties"') as $file) {
		$file = substr($file, 2);
		$count = count_strings($file);
		Files::ReplaceFile($repo, 'en-US', $file, $count, 0);
	}
	DBConnection::CommitTransaction();

	chdir($old_dir);
}

function count_strings($file) {
	global $script_root;

	$json = run($script_root.'/parse.py '.$file);
	$json = $json[0];
	$data = json_decode($json, true);
	return sizeof($data);
}
