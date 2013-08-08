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

function get_base_strings() {
	global $db_repo, $script_root, $repo_path, $repo_locale_path, $file;

	if ($db_repo['jetpack']) {
		$path = $repo_path.'/master/'.$repo_locale_path.'/en-US.properties';
	} else {
		$path = $repo_path.'/master/'.$repo_locale_path.'/en-US/'.$file;
	}

	$json = run($script_root.'/parse.py '.escapeshellarg($path));
	return json_decode($json[0], true);
}
