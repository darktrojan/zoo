<?

class InstallRDF {
	const RDF_NS = 'http://www.w3.org/1999/02/22-rdf-syntax-ns#';
	const EM_NS = 'http://www.mozilla.org/2004/em-rdf#';

	function __construct($path) {
		$this->path = $path;
		$this->dom = DomDocument::load($this->path);
		$this->dom->encoding = 'utf-8';
	}

	function cat() {
		return $this->dom->saveXML($dom);
	}

	function save() {
		file_put_contents($this->path, $this->cat());
	}

	function get_name() {
		return InstallRDF::get_text_content_at($this->dom->documentElement, '/x:RDF/x:Description/em:name');
	}

	function get_description() {
		return InstallRDF::get_text_content_at($this->dom->documentElement, '/x:RDF/x:Description/em:description');
	}

	function get_locale_nodes() {
		$nodes = array();
		foreach ($this->xpath($this->dom->documentElement, '/x:RDF/x:Description/em:localized') as $locale_node) {
			$locale = InstallRDF::get_text_content_at($locale_node, 'x:Description/em:locale');
			$nodes[$locale] = $locale_node;
		}
		return $nodes;
	}

	function locale_get_name($locale_node) {
		return InstallRDF::get_text_content_at($locale_node, 'x:Description/em:name');
	}

	function locale_get_description($locale_node) {
		return InstallRDF::get_text_content_at($locale_node, 'x:Description/em:description');
	}

	function locale_set_name($locale_node, $name) {
		InstallRDF::set_text_content_at($locale_node, 'x:Description/em:name', $name);
	}

	function locale_set_description($locale_node, $description) {
		InstallRDF::set_text_content_at($locale_node, 'x:Description/em:description', $description);
	}

	function locale_add($locale, $name, $description) {
		$parent = InstallRDF::xpath($this->dom->documentElement, '/x:RDF/x:Description')->item(0);
		$outer_locale_node = $this->dom->createElementNS(InstallRDF::EM_NS, 'localized');
		$outer_description_node = $this->dom->createElementNS(InstallRDF::RDF_NS, 'Description');
		$locale_node = $this->dom->createElementNS(InstallRDF::EM_NS, 'locale');
		$locale_text = $this->dom->createTextNode($locale);
		$name_node = $this->dom->createElementNS(InstallRDF::EM_NS, 'name');
		$name_text = $this->dom->createTextNode($name);
		$description_node = $this->dom->createElementNS(InstallRDF::EM_NS, 'description');
		$description_text = $this->dom->createTextNode($description);

		$parent->appendChild($outer_locale_node);
		$outer_locale_node->appendChild($outer_description_node);
		$outer_description_node->appendChild($locale_node);
		$locale_node->appendChild($locale_text);
		$outer_description_node->appendChild($name_node);
		$name_node->appendChild($name_text);
		$outer_description_node->appendChild($description_node);
		$description_node->appendChild($description_text);

		return $outer_locale_node;
	}

	private static function xpath($node, $xpath) {
		$dom_xpath = new DOMXPath($node->ownerDocument);
		$dom_xpath->registerNamespace('x', InstallRDF::RDF_NS);
		$dom_xpath->registerNamespace('em', InstallRDF::EM_NS);
		return $dom_xpath->query($xpath, $node);
	}

	private static function get_text_content_at($node, $xpath) {
		foreach (InstallRDF::xpath($node, $xpath) as $element) {
			return $element->textContent;
		}
		return false;
	}

	private static function set_text_content_at($node, $xpath, $text) {
		foreach (InstallRDF::xpath($node, $xpath) as $element) {
			$element->replaceChild($node->ownerDocument->createTextNode($text), $element->firstChild);
			return;
		}
	}
}
