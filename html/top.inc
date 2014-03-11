<?

require_once 'hidden_funcs.inc';

$site_root = dirname(dirname(__FILE__));
$http_root = $site_root.'/html';
$script_root = $site_root.'/scripts';
$repo_root = $site_root.'/repos';
$github_user = 'darktrojan-test';

date_default_timezone_set('Pacific/Auckland');

$context = array();
$scripts = array();
$commands = array();
$context['scripts'] = &$scripts;
$context['commands'] = &$commands;

if (isset($_COOKIE['email'], $_COOKIE['session']) &&
		check_session_hash($_COOKIE['email'], $_COOKIE['session'])) {
	$session_user = $_COOKIE['email'];
}

if ($session_user) {
	if ($db_user = Users::SelectOneUserByEmail($session_user)) {
		$db_user['locales'] = $db_user['locale'] ? explode(',', $db_user['locale']) : array();
		$context['db_user'] = &$db_user;
	}
}

$scripts[] = 'https://login.persona.org/include.js';
$scripts[] = '/persona/persona.js';
$scripts[] = '/persona/xhr.js';

function check_session_hash($email, $hash) {
	$string = create_session_hash($email);
	if ($hash == $string) {
		return true;
	}
	if (idate('i') < 30) {
		$string = create_session_hash($email, true);
		if ($hash == $string) {
			set_session_cookies($email);
			return true;
		}
	}
	clear_session_cookies();
	return false;
}

function set_session_cookies($email) {
	$hash = create_session_hash($email);
	global $site_host;

	setcookie('email', $email, time() + 3600, '/', $host, false, false);
	setcookie('session', $hash, time() + 3600, '/', $host, false, true);
}

function clear_session_cookies() {
	global $site_host;

	setcookie('email', '', time() - 3600, '/', $host, false, false);
	setcookie('session', '', time() - 3600, '/', $host, false, true);
	unset($_COOKIE['session'], $_COOKIE['email']);
}

function get_locales() {
	global $site_root, $global_locales, $context;

	if (!isset($global_locales)) {
		$global_locales = array();
		$f = fopen($site_root.'/locales.csv', 'r');
		while ($l = fgetcsv($f, "\n", "\t")) {
			if ($l[0][0] != '#') {
				$global_locales[$l[0]] = $l[1];
			}
		}
		fclose($f);
		$context['global_locales'] = &$global_locales;
	}
}

function get_locale_name($locale = null) {
	global $global_locales;

	if ($locale == null) {
		$locale = $GLOBALS['locale'];
	}

	get_locales();
	return $global_locales[$locale] ?: $locale;
}

// Only call this on the master workdir
function run_parse_py($path) {
	global $script_root;

	$cachePath = $path.'.zoojson';
	if (file_exists($cachePath) && filemtime($cachePath) > filemtime($path)) {
		return file_get_contents($cachePath);
	}

	$json = run($script_root.'/parse.py '.escapeshellarg($path));
	// $json is an array of lines of output
	$json = implode('', $json);
	file_put_contents($cachePath, $json);
	return $json;
}

// Don't call this on the master workdir
function run_parse_strings_py($path) {
	global $script_root;

	if (!file_exists($path)) {
		return '';
	}

	$cachePath = $path.'.zoojson';
	if (file_exists($cachePath) && filemtime($cachePath) > filemtime($path)) {
		return file_get_contents($cachePath);
	}

	$json = run($script_root.'/parse_strings.py '.escapeshellarg($path));
	// $json is an array of lines of output
	$json = implode('', $json);
	file_put_contents($cachePath, $json);
	return $json;
}
