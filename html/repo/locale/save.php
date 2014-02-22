<?

if (!isset($_REQUEST['file'])) {
	exit('not enough args');
}
$file = $_REQUEST['file'];

if (!$locale_user_is_translator) {
	exit('not authorized');
}

$locale_strings = $_POST['locale_strings'];
if ($file == 'install.rdf') {
	$installRDF = new InstallRDF($locale_workdir.'/install.rdf');

	if (array_search($locale, $installRDF->get_locales()) !== false) {
		$installRDF->locale_set_name($locale, $locale_strings['name']);
		$installRDF->locale_set_description($locale, $locale_strings['description']);
	} else {
		$installRDF->locale_add($locale, $locale_strings['name'], $locale_strings['description']);
	}
	$installRDF->save();
	chdir($locale_workdir);
	run('git add install.rdf');

	$count = 2;
	$same = 0;
} else {
	$base_strings = get_base_strings();

	ob_start();
	if (preg_match('/\.dtd$/', $file)) {
		$format = '%s%s<!ENTITY %s "%s">%s';
	} else {
		$format = "%s%s%s = %s\n%s";
	}

	$count = 0;
	$same = 0;
	foreach ($base_strings as $entity) {
		$locale_value = $locale_strings[$entity['key']];
		if (preg_match('/\.dtd$/', $file)) {
			$locale_value = htmlspecialchars($locale_value, ENT_COMPAT, 'utf-8');
		}

		if ($locale_strings[$entity['key']]) {
			$count++;
		}
		if ($entity['value'] == $locale_value) {
			$same++;
		}
		printf(
			$format,
			$entity['pre_ws'],
			$entity['pre_comment'],
			$entity['key'],
			$locale_value,
			$entity['post']
		);
	}
	$contents = ob_get_contents();
	ob_end_clean();

	if ($db_repo['jetpack']) {
		file_put_contents($locale_path_abs.'/'.$locale.'.properties', $contents);
		chdir($locale_path_abs);
		run('git add '.escapeshellarg($locale.'.properties'));
		$file = 'en-US.properties';
	} else {
		ensure_directory(dirname($file));
		file_put_contents($locale_path_abs.'/'.$locale.'/'.$file, $contents);
		chdir($locale_path_abs);
		run('git add '.escapeshellarg($locale.'/'.$file));
	}
}

Files::ReplaceFile($repo, $locale, $file, $count, $same);
Log::InsertLogItem($db_user['id'], 'locale_update', $repo, $locale);

header('Location: locale.php');

function ensure_directory($directory) {
	global $locale_path_abs, $locale;

	$directory_abs = $locale_path_abs.'/'.$locale.'/'.$directory;
	if ($directory == '.' || is_dir($directory_abs)) {
		return;
	}
	ensure_directory(dirname($directory));
	mkdir($directory_abs);
}
