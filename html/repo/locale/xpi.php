<?

$output_http = sprintf('/xpi/%s_%s_%s.xpi', $repo_owner, $repo_name, $locale);
$output_abs = $http_root.$output_http;

if (isset($_POST['command'])) {
	$command = $_POST['command'];

	switch ($command) {
	case 'zip':
		chdir($locale_xpi_path_abs);
		run_xhr(sprintf(
			'zip -r %s * -x .git', escapeshellarg($output_abs)
		));
		break;
	case 'cfx':
		$cfx_path = $script_root.'/addon-sdk/bin/cfx';
		run_xhr(sprintf(
			'%s xpi --pkgdir %s --output-file %s',
			$cfx_path,
			escapeshellarg($locale_xpi_path_abs),
			escapeshellarg($output_abs)
		));
		break;
	}

	exit;
}

$commands[] = array(
	'string' => 'Packaging',
	'name' => $db_repo['jetpack'] ? 'cfx' : 'zip'
);

require_once 'twig.inc';
render('repo/locale/xpi.twig', compact('output_http'));
