<?

$output_http = sprintf('/xpi/%s_%s_%s.xpi', $repo_owner, $repo_name, $locale);
$output_abs = $site_root.'/html'.$output_http;

if (isset($_POST['command'])) {
	$command = $_POST['command'];

	switch ($command) {
	case 'zip':
		chdir($locale_xpi_path_abs);
		run_xhr(sprintf(
			'zip -r %s * -x .git', escapeshellarg($output_abs)
		));
		break;
	}

	exit;
}

$commands[] = array(
	'string' => 'Packaging',
	'name' => 'zip'
);

require_once 'twig.inc';
render('repo/locale/xpi.twig', compact('output_http'));
