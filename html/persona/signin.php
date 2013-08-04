<?

$output = run('curl -d '.escapeshellarg('assertion='.$_POST['assertion'].'&audience='.$site_host).
	' https://verifier.login.persona.org/verify', false, $retVal);

if ($retVal == 0) {
	$output = implode("\n", $output);
	$data = json_decode($output, true);

	if ($data['status'] == 'okay') {
		setSessionCookies($data['email']);

		$db_user = Users::SelectOneUserByEmail($data['email']);
		if ($db_user) {
			echo 'login';
		} else {
			echo 'new';
		}
		exit;
	}
}

header('HTTP/1.1 500 Error', true, 500);
exit;
