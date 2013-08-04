<?

if (!$locale_user_is_translator) {
	exit('not authorized');
}

if (!isset($_GET['file'])) {
	exit('not enough args');
}

$file = $_GET['file'];

$base_strings = get_base_strings();
$locale_strings = get_locale_strings();

$strings = array();
foreach ($base_strings as $entity) {
	$comment = $entity['pre_comment'];
		if ($comment) {
		$comment = preg_replace('/\n/', ' ', $comment);
		$comment = preg_replace('/<!--\s*(.*)\s*-->/', '\1', $comment);
		$comment = preg_replace('/^#\s*/', '', $comment);
	}
	$locale_string = $locale_strings[$entity['key']];

	$strings[] = array(
		'key' => $entity['key'],
		'comment' => $comment,
		'base_value' => $entity['value'],
		'locale_value' => $locale_string
	);
}

require_once 'twig.inc';
render('repo/locale/edit.twig', compact('file', 'strings'));
