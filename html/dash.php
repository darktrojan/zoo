<?

if (isset($db_user)) {
	$repos = Repos::SelectReposByUser($db_user['id']);

	get_locales();
	$translations = Translations::SelectTranslationsByUser($db_user['id']);
	foreach ($translations as &$translation) {
		$repo = Repos::SelectOneRepo($translation['repo']);
		$translation['repo_name'] = $repo['name'];
		$translation['locale_name'] = $global_locales[$translation['locale']];
	}
}

require_once 'twig.inc';
render('dash.twig', compact('repos', 'translations', 'global_locales'));
