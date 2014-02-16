<?

if (!isset($_GET['repo'])) {
	exit('not enough args');
}

$repo = $_GET['repo'];
if (!preg_match('/^[\w-]+\/[\w-]+$/', $repo)) {
	exit('not a valid repo');
}

$repo_owner = dirname($repo);
$repo_name = basename($repo);

require_once dirname(dirname(__FILE__)).'/top.inc';

$db_repo = Repos::SelectOneRepo($repo);

$repo_user_is_owner = isset($db_user) && $db_user['github'] == $repo_owner;
$repo_path = $repo_root.'/'.$repo;
if (!is_dir($repo_path)) {
	exit("repo doesn't exist");
}

$repo_locale_path = $db_repo['locale_path'];
$repo_files = array();
$db_files = Files::SelectFilesByLocale($repo, 'en-US');
foreach ($db_files as $file) {
	$repo_files[] = $file['file'];
}

$context['repo'] = array(
	'fullname' => $repo,
	'owner' => $repo_owner,
	'name' => $repo_name,
	'user_is_owner' => $repo_user_is_owner,
	'files' => $db_files,
	'filenames' => $repo_files,
	'branch' => $db_repo['branch']
);
$context['db_repo'] = $db_repo;

function get_base_strings() {
	global $db_repo, $script_root, $repo_path, $repo_locale_path, $file;

	if ($db_repo['jetpack']) {
		$path = $repo_path.'/master/'.$repo_locale_path.'/en-US.properties';
	} else {
		$path = $repo_path.'/master/'.$repo_locale_path.'/en-US/'.$file;
	}

	$json = run_parse_py($path);
	return json_decode($json, true);
}

// NB: $locale_path_abs and $locale must be set before calling get_locale_strings.
// From inside a locale, these are automatically set.
function get_locale_strings() {
	global $db_repo, $script_root, $locale_path_abs, $locale, $file;

	if ($db_repo['jetpack']) {
		$path = $locale_path_abs.'/'.$locale.'.properties';
	} else {
		$path = $locale_path_abs.'/'.$locale.'/'.$file;
	}
	$json = run_parse_strings_py($path);
	return json_decode($json, true);
}

function update_translations($locales) {
	global $db_files, $file, $locale, $repo_path, $repo_locale_path, $locale_path_abs, $repo;

	DBConnection::BeginTransaction();
	foreach ($db_files as $db_file) {
		$file = $db_file['file'];
		$base_strings = get_base_strings();
		Files::ReplaceFile($repo, 'en-US', $file, sizeof($base_strings), 0);
		foreach ($locales as $locale) {
			if ($locale == 'en-US') {
				continue;
			}
			$locale_workdir = $repo_path.'/'.$locale;
			$locale_path_abs = $locale_workdir.'/'.$repo_locale_path;
			$locale_strings = get_locale_strings();
			$ok = 0;
			$same = 0;
			$missing = 0;
			foreach ($base_strings as $base_string) {
				$key = $base_string['key'];
				$value = $base_string['value'];
				$locale_string = $locale_strings[$key];

				if (!$locale_string) {
					$missing++;
				} elseif ($locale_string == $value) {
					$same++;
				} else {
					$ok++;
				}
			}

			var_dump($repo, $locale, $file, $ok + $same, $same);
			Files::ReplaceFile($repo, $locale, $file, $ok + $same, $same);
		}
	}
	DBConnection::CommitTransaction();
}
