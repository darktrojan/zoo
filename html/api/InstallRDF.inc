<?

class InstallRDF {
	function __construct($path) {
		$this->path = $path;
		$this->xml = simplexml_load_file($this->path);
		$this->xml->registerXPathNamespace('x', 'http://www.w3.org/1999/02/22-rdf-syntax-ns#');
		$this->xml->registerXPathNamespace('em', 'http://www.mozilla.org/2004/em-rdf#');
	}

	function cat() {
		$dom_sxe = dom_import_simplexml($this->xml);
		$dom = new DOMDocument('1.0');
		$dom_sxe = $dom->importNode($dom_sxe, true);
		$dom_sxe = $dom->appendChild($dom_sxe);

		return $dom->saveXML($dom);
	}

	function save() {
		file_put_contents($this->path, $this->cat());
	}

	function get_name() {
		foreach ($this->xml->xpath('/x:RDF/x:Description/em:name') as $name) {
			return dom_import_simplexml($name)->textContent;
		}
	}
}
