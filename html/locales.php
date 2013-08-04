<?

get_locales();
$summary = Files::SelectSummaryAll();

require_once 'twig.inc';
render('locales.twig', compact('summary'));
