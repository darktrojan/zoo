<?

get_locales();

$log = Log::SelectLogItems();
foreach ($log as &$entry) {
	$entry['db_user'] = Users::SelectOneUser($entry['user']);
}

require_once 'twig.inc';
render('log.twig', compact('log'));
