<?

if ($locale_user_is_translator) {
	chdir($locale_workdir);
	$status = run('git status --short');

	$dirty = sizeof($status);
}

$files = array();
foreach (Files::SelectFilesByLocale($repo, $locale) as $file) {
	if ($db_repo['jetpack']) {
		$file['file'] = $locale.'.properties';
		$files['en-US.properties'] = $file;
	} else {
		$files[$file['file']] = $file;
	}
}

require_once 'twig.inc';
render('repo/locale/locale.twig', compact('repo_files', 'files', 'dirty', 'db_locale'));
