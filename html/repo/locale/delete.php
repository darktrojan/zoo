<?

if (!$locale_user_is_translator) {
	exit('not authorized');
}

$locales = explode(',', $db_repo['locales']);
$index = array_search($locale, $locales);
if ($index !== false) {
	array_splice($locales, $index, 1);
	Repos::UpdateRepoLocales($repo, implode(',', $locales));
}

Translations::DeleteTranslation($repo, $locale);
foreach ($repo_files as $file) {
	Files::DeleteFile($repo, $locale, $file);
}

chdir($repo_path.'/master');
run_xhr('git branch -D '.$locale, false);
chdir($repo_path);
run_xhr('rm -rf '.$locale);
