<?

get_locales();
$repos = Repos::SelectRepos();
foreach ($repos as &$r) {
	if ($r['locales']) {
		$r['total_count'] = sizeof(explode(',', $r['locales']));
		$r['managed_count'] = sizeof(Translations::SelectTranslationsByRepo($r['path']));
	} else {
		$r['total_count'] = 0;
		$r['managed_count'] = 0;
	}
}

require_once 'twig.inc';
render('repos.twig', compact('repos'));
