<?

class ChromeManifest {
	const pattern = '/locale\s+\w+\s+([a-z]{2}(-[A-Z]{2})?)(\s+)(.*)$/';

	function __construct($path, $xpi_path = '') {
		$this->path = $path;
		$this->lines = file($this->path, FILE_IGNORE_NEW_LINES);
		$this->locales = array();

		foreach ($this->lines as $line) {
			if (preg_match(ChromeManifest::pattern, $line, $parts)) {
				if ($parts[1] == 'en-US') {
					$this->locale_path = preg_replace('|/en-US/.*|', '', $parts[4]);
					if ($this->xpi_path) {
						$this->locale_path = $this->xpi_path.'/'.$this->locale_path;
					}
				} else {
					$this->locales[] = $parts[1];
				}
			}
		}
	}

	function cat() {
		return implode("\n", $this->lines)."\n";
	}

	function save() {
		file_put_contents($this->path, $this->cat());
	}

	function add_locale($add) {
		$add_dir = $add.'/';
		$is_alphabetical = $this->locales_alphabetical();

		$i = $is_alphabetical ? 0 : 1;
		foreach ($this->lines as $line) {
			if (preg_match(ChromeManifest::pattern, $line, $parts)) {
				if ($parts[1] == 'en-US') {
					$locale_line = $parts[0];
				} else {
					if (strlen($add) < 5 && strlen($parts[1]) < 5 && strlen($parts[3]) > 1 && $parts[3][0] != "\t") {
						$add = str_pad($add, 5);
					}
					if (!$is_alphabetical || !isset($insert_index)) {
						$insert_index = $i;
					}
					if ($is_alphabetical) {
						$comparison = strcasecmp($parts[1], $add);
						if ($comparison == 0) {
							return;
						}
						if ($comparison < 0) {
							$insert_index = $i + 1;
						}
					}
				}
			}
			$i++;
		}

		if (!isset($insert_index)) {
			$insert_index = sizeof($this->lines);
		}

		$new_locale_line = str_replace(array('en-US/', 'en-US'), array($add_dir, $add), $locale_line);
		array_splice($this->lines, $insert_index, 0, $new_locale_line);
	}

	private function locales_alphabetical() {
		$alphabetical = array_slice($this->locales, 0);
		sort($alphabetical);

		for ($i = 0; $i < sizeof($alphabetical); $i++) {
			if ($this->locales[$i] != $alphabetical[$i]) {
				return false;
			}
		}
		return true;
	}
}
