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

	$xpi_path = '';

	// TODO something useful here
	$manifest = run('find -name "chrome.manifest"');
	if (sizeof($manifest) == 1) {
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

		$locale_dir = $repo_path.'/master/'.$repo_locale_path.'/en-US';
		chdir($locale_dir);
		$files = array();
		foreach (run('find -name "*.dtd" -o -name "*.properties"') as $file) {
			$file = substr($file, 2);
			$count = count_strings($file);
			$files[$file] = $count;
		}
	} else {
		$jetpack_manifest = run('find -name "package.json"');
		if (sizeof($jetpack_manifest) == 1) {
			$jetpack_manifest = substr($jetpack_manifest[0], 2);
			if ($jetpack_manifest != 'package.json') {
				$xpi_path = dirname($jetpack_manifest);
				chdir($xpi_path);
			}

			$json = json_decode(file_get_contents($jetpack_manifest), true);
			$name = $json['fullName'] ?: $json['name'];
			$repo_locale_path = $xpi_path ? $xpi_path.'/locale' : 'locale';

			$locales = array();
			chdir($repo_locale_path);
			foreach (glob('*.properties') as $l) {
				$locales[] = substr(basename($l), 0, -11);
			}

			$index = array_search('en-US', $locales);
			if ($index !== false) {
				array_splice($locales, $index, 1);
			} else {
				exit('en-US translation not found');
			}

			$files = array('en-US.properties' => count_strings('en-US.properties'));
		} else {
			exit('need exactly 1 chrome.manifest, '.sizeof($manifest).' found');
		}
	}

	foreach (Translations::SelectTranslationsByRepo($repo) as $t) {
		$locales[] = $t['locale'];
	}
	$locales = array_unique($locales);

	DBConnection::BeginTransaction();
	if ($update) {
		Repos::UpdateRepo(
			$repo, $name, isset($jetpack_manifest), implode(',', $locales), $xpi_path, $repo_locale_path, $branch
		);
		Log::InsertLogItem($db_user['id'], 'repo_update', $repo, '');
	} else {
		Repos::InsertRepo(
			$db_user['id'], $repo, $name, isset($jetpack_manifest), implode(',', $locales), $xpi_path, $repo_locale_path, $branch
		);
		Log::InsertLogItem($db_user['id'], 'repo_new', $repo, '');
	}

	foreach ($files as $file => $count) {
		Files::ReplaceFile($repo, 'en-US', $file, $count, 0);
	}
	DBConnection::CommitTransaction();

	chdir($old_dir);
}

function count_strings($file) {
	global $script_root;

	$json = run_parse_py($file);
	$json = $json[0];
	$data = json_decode($json, true);
	return sizeof($data);
}
