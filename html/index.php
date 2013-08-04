<?

$repos = Repos::SelectRepos();
$summary = Files::SelectSummaryAll();

if (isset($db_user)) {
	$myrepos = Repos::SelectReposByUser($db_user['id']);
	$mytranslations = Translations::SelectTranslationsByUser($db_user['id']);
	foreach ($mytranslations as &$translation) {
		$translation['locale_name'] = get_locale_name($translation['locale']);
	}
}

require_once 'twig.inc';
render('index.twig', compact('repos', 'summary', 'myrepos', 'mytranslations'));
