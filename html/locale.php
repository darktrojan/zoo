<?

if (!isset($_GET['locale'])) {
	exit('not enough args');
}

$locale = $_GET['locale'];

get_locales();

require_once 'twig.inc';
render('locale.twig', array(
	'locale_name' => $global_locales[$locale] ?: $locale,
	'repos' => Repos::SelectRepos(),
	'missing' => Repos::SelectReposWithoutTranslation($locale),
	'translators' => Translations::SelectTranslatorsByLocale($locale)
));
