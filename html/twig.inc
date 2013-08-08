<?

require_once 'twig/lib/Twig/Autoloader.php';
Twig_Autoloader::register();

$loader = new Twig_Loader_Filesystem($GLOBALS['http_root']);
$config = array();

$GLOBALS['twig'] = new Twig_Environment($loader, $config);

$GLOBALS['twig']->addFilter(new Twig_SimpleFilter('pluralize', function($value, $plural = 's', $singular = '') {
	return $value == 1 ? $singular : $plural;
}));

function render($template, $extra_context = array()) {
	global $context;

	header('Content-Type: text/html;charset=utf-8');

	$context = array_merge($context, $extra_context);
	$t = $GLOBALS['twig']->loadTemplate($template);
	echo $t->render($context);
}
