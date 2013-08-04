<?

if (isset($db_user)) {
	if (!!$db_user['github'] && is_dir($repo_root.'/'.$db_user['github'])) {
		$repos = array();
		$dir = opendir($repo_root.'/'.$db_user['github']);
		while ($file = readdir($dir)) {
			if ($file[0] == '.' || !is_dir($repo_root.'/'.$db_user['github'].'/'.$file)) {
				continue;
			}
			$repos[] = $file;
		}
		closedir($dir);
	}

	get_locales();
	$translations = Translations::SelectTranslationsByUser($db_user['id']);
	foreach ($translations as &$translation) {
		$translation['locale_name'] = $global_locales[$translation['locale']];
	}
}

require_once 'twig.inc';
render('dash.twig', compact('repos', 'translations', 'global_locales'));
