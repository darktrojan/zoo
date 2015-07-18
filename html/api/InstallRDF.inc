<?

class InstallRDF {
	const RDF_NS = 'http://www.w3.org/1999/02/22-rdf-syntax-ns#';
	const EM_NS = 'http://www.mozilla.org/2004/em-rdf#';

	private $nl = "\n";
	private $tab = '  ';

	private $path, $dom, $locale_nodes;

	function __construct($path) {
		$this->path = $path;
		$this->dom = DomDocument::load($this->path);
		$this->dom->encoding = 'utf-8';

		$whitespace = $this->dom->documentElement->firstChild;
		if ($whitespace instanceof DOMText) {
			$whitespace = $whitespace->nodeValue;
			if (substr($whitespace, 0, 2) == "\r\n") {
				$this->nl = "\r\n";
			}
			$this->tab = ltrim($whitespace, "\r\n");
		} else {
			$this->nl = '';
			$this->tab = '';
		}
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

	private function store_locale_nodes() {
		if (is_array($this->locale_nodes)) {
			return;
		}

		$this->locale_nodes = array();
		foreach ($this->xpath($this->dom->documentElement, '/x:RDF/x:Description/em:localized') as $locale_node) {
			$locale = InstallRDF::get_text_content_at($locale_node, 'x:Description/em:locale');
			$this->locale_nodes[$locale] = $locale_node;
		}
	}

	function get_locales() {
		$this->store_locale_nodes();
		return array_keys($this->locale_nodes);
	}

	function locale_get_name($locale) {
		$this->store_locale_nodes();

		if ($locale_node = $this->locale_nodes[$locale]) {
			return InstallRDF::get_text_content_at($locale_node, 'x:Description/em:name');
		}
		return false;
	}

	function locale_get_description($locale) {
		$this->store_locale_nodes();

		if ($locale_node = $this->locale_nodes[$locale]) {
			return InstallRDF::get_text_content_at($locale_node, 'x:Description/em:description');
		}
		return false;
	}

	function locale_set_name($locale, $name) {
		$this->store_locale_nodes();

		if ($locale_node = $this->locale_nodes[$locale]) {
			InstallRDF::set_text_content_at($locale_node, 'x:Description/em:name', $name);
		}
	}

	function locale_set_description($locale, $description) {
		$this->store_locale_nodes();

		if ($locale_node = $this->locale_nodes[$locale]) {
			InstallRDF::set_text_content_at($locale_node, 'x:Description/em:description', $description);
		}
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

		$last = $parent->lastChild;
		if ($last instanceof DOMText && !trim($last->nodeValue)) {
			$parent->removeChild($last);
		}

		$parent->appendChild($this->dom->createTextNode(str_repeat($this->nl, 2).str_repeat($this->tab, 2)));
		$parent->appendChild($outer_locale_node);
		$outer_locale_node->appendChild($this->dom->createTextNode($this->nl.str_repeat($this->tab, 3)));
		$outer_locale_node->appendChild($outer_description_node);
		$outer_description_node->appendChild($this->dom->createTextNode($this->nl.str_repeat($this->tab, 4)));
		$outer_description_node->appendChild($locale_node);
		$locale_node->appendChild($locale_text);
		$outer_description_node->appendChild($this->dom->createTextNode($this->nl.str_repeat($this->tab, 4)));
		$outer_description_node->appendChild($name_node);
		$name_node->appendChild($name_text);
		$outer_description_node->appendChild($this->dom->createTextNode($this->nl.str_repeat($this->tab, 4)));
		$outer_description_node->appendChild($description_node);
		$outer_description_node->appendChild($this->dom->createTextNode($this->nl.str_repeat($this->tab, 3)));
		$outer_locale_node->appendChild($this->dom->createTextNode($this->nl.str_repeat($this->tab, 2)));
		$description_node->appendChild($description_text);
		$parent->appendChild($this->dom->createTextNode($this->nl.str_repeat($this->tab, 1)));

		$this->locale_nodes[$locale] = $outer_locale_node;
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
			$textNode = $node->ownerDocument->createTextNode($text);
			if ($element->firstChild) {
				$element->replaceChild($textNode, $element->firstChild);
			} else {
				$element->appendChild($textNode);
			}
			return;
		}
	}
}
