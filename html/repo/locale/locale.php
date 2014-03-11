<?

if ($locale_user_is_translator) {
	chdir($locale_workdir);
	$status = run('git status --short -u no');

	$dirty = sizeof($status);
}

$base_files = Files::SelectFilesByLocale($repo, 'en-US');

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
render('repo/locale/locale.twig', compact('base_files', 'repo_files', 'files', 'dirty', 'db_locale'));
