<?

function git_add_all() {
	global $repo_locale_path, $locale, $repo_files;

	foreach ($repo_files as $file) {
		if ($file != 'install.rdf') {
			run_xhr('git add '.escapeshellarg($repo_locale_path.'/'.$locale.'/'.$file));
		}
	}
	if (file_exists('install.rdf')) {
		run_xhr('git add install.rdf');
	}
	if (file_exists('chrome.manifest')) {
		run_xhr('git add chrome.manifest');
	}
}

function git_modified() {
	$modified = array();
	foreach (run('git status --porcelain') as $line) {
		if ($line[0] != '?') {
			$modified[] = substr($line, 3);
		}
	}
	return $modified;
}

// Must be run in branch workdir
function git_rebase() {
	global $branch, $locale;

	$modified = git_modified();

	// Stash changes
	$ir_modified = array_search('install.rdf', $modified);
	if ($ir_modified !== false) {
		$ir = new InstallRDF('install.rdf');
		$ir_name = $ir->locale_get_name($locale);
		$ir_desc = $ir->locale_get_description($locale);
		run_xhr('git reset HEAD install.rdf 2>&1', false);
		run_xhr('git checkout -- install.rdf 2>&1', false);
		array_splice($modified, $ir_modified, 1);
	}
	$cm_modified = array_search('chrome.manifest', $modified);
	if ($cm_modified !== false) {
		run_xhr('git reset HEAD chrome.manifest 2>&1', false);
		run_xhr('git checkout -- chrome.manifest 2>&1', false);
		array_splice($modified, $cm_modified, 1);
	}
	if (sizeof($modified)) {
		run_xhr('git stash 2>&1');
	}

	// Do rebase
	run_xhr('git rebase '.escapeshellarg($branch).' 2>&1', false, $retVal);
	if ($retVal != 0) {
		run_xhr('git rebase --abort', false);
	}

	// Unstash changes
	if (sizeof($modified)) {
		run_xhr('git stash pop 2>&1', false);
	}
	if ($ir_modified !== false) {
		$ir = new InstallRDF('install.rdf');
		if (array_search($locale, $ir->get_locales()) != false) {
			$ir->locale_set_name($locale, $ir_name);
			$ir->locale_set_description($locale, $ir_desc);
		} else {
			$ir->locale_add($locale, $ir_name, $ir_desc);
		}
		$ir->save();
	}
	if ($cm_modified !== false) {
		$cm = new ChromeManifest('chrome.manifest');
		$cm->add_locale($locale);
		$cm->save();
	}
}
