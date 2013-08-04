<?

if (isset($db_user)) {
	header('Location: /');
	exit;
}

if (isset($_POST['email'], $_POST['name'])) {
	Users::InsertUser($_POST['email'], $_POST['name'], '', '');
	if (isset($_POST['referer'])) {
		header('Location: '.$_POST['referer']);
	} else {
		header('Location: /');
	}
	exit;
}

$referer = $_SERVER['HTTP_REFERER'];

require_once 'twig.inc';
render('persona/new.twig', compact('session_user', 'referer'));
