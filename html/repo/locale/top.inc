<?

if (!isset($_GET['locale'])) {
	exit('not enough args');
}

$locale = $_GET['locale'];
if (!preg_match('/^[a-z]{2}(-[A-Z]{2})?$/', $locale)) {
	exit('not a valid locale');
}

require_once dirname(dirname(__FILE__)).'/top.inc';

$db_locale = Translations::SelectOneTranslation($repo, $locale);

$locale_workdir = $repo_path.'/'.$locale;
$locale_path_abs = $locale_workdir.'/'.$repo_locale_path;
$locale_xpi_path_abs = $locale_workdir;
if ($db_repo['xpi_path']) {
	$locale_xpi_path_abs .= '/'.$db_repo['xpi_path'];
}

@chdir($locale_workdir) or exit('locale doesn\'t exist');

if ($db_locale) {
	$locale_translator = $db_locale['user'];
}

$locale_user_is_translator = $repo_user_is_owner ||
	isset($db_user, $locale_translator) && $db_user['id'] == $locale_translator;

$context['locale'] = array(
	'translator' => $locale_translator,
	'user_is_translator' => $locale_user_is_translator,
	'name' => get_locale_name()
);

if ($db_user['id'] == $locale_translator) {
	$context['locale']['db_translator'] = &$db_user;
} else {
	$context['locale']['db_translator'] = Users::SelectOneUser($locale_translator);
}

function git($command) {
	global $locale_workdir, $github_user;

	$command = sprintf($command, $github_user);

	return sprintf(
		'git %s 2>&1',
		$command
	);
}

function get_locale_strings() {
	global $db_repo, $script_root, $locale_path_abs, $locale, $file;

	if ($db_repo['jetpack']) {
		$path = $locale_path_abs.'/'.$locale.'.properties';
	} else {
		$path = $locale_path_abs.'/'.$locale.'/'.$file;
	}
	$json = run($script_root.'/parse_strings.py '.escapeshellarg($path));
	return json_decode($json[0], true);
}
