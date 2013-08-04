<?

get_locales();
$context['global_locales'] = $global_locales;

$summary = array();
foreach (Files::SelectSummary($repo) as $l) {
	$summary[$l['locale']] = $l;
}

$base_summary = $summary['en-US'];

$repo_locales = array();
if ($db_repo['locales']) {
	foreach (explode(',', $db_repo['locales']) as $l) {
		$repo_locales[$l] = array(
			'key' => $l,
			'new' => true,
			'name' => $global_locales[$l]
		);
	}
}

foreach(Translations::SelectTranslationsByRepo($repo) as $t) {
	$l = $t['locale'];
	$repo_locales[$l] = array(
		'key' => $l,
		'new' => false,
		'name' => $global_locales[$l],
		'summary' => $summary[$l]
	);
}

uasort($repo_locales, function($a, $b) {
	return strcasecmp($a['name'] ?: $a['key'], $b['name'] ?: $b['key']);
});

require_once 'twig.inc';
render('repo/repo.twig', compact('repo_locales', 'base_summary'));
