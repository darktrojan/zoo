<?

class Translations extends DBConnection {
	static function SelectTranslations() {
		$stmt = self::CreateStatement('SELECT id, repo, locale, user, pullrequest FROM translations');
		self::RunStatement($stmt);
		return $stmt->fetchAll();
	}

	static function SelectOneTranslation($repo, $locale) {
		$stmt = self::CreateStatement('SELECT id, user, pullrequest FROM translations WHERE repo = :repo AND locale = :locale');
		self::RunStatement(
			$stmt, array(
				':repo' => $repo,
				':locale' => $locale
			)
		);
		return $stmt->fetch() ?: null;
	}

	static function SelectTranslationsByUser($user) {
		$user = (int)$user;

		$stmt = self::CreateStatement('SELECT id, repo, locale, pullrequest FROM translations WHERE user = :user');
		self::RunStatement(
			$stmt, array(
				':user' => $user
			)
		);
		return $stmt->fetchAll();
	}

	static function SelectTranslationsByRepo($repo) {
		$stmt = self::CreateStatement('SELECT id, locale, user, pullrequest FROM translations WHERE repo = :repo');
		self::RunStatement(
			$stmt, array(
				':repo' => $repo
			)
		);
		return $stmt->fetchAll();
	}

	static function SelectTranslatorsByLocale($locale) {
		$stmt = self::CreateStatement('SELECT DISTINCT u.name, u.email FROM translations t JOIN users u ON t.user = u.id WHERE t.locale = :locale');
		self::RunStatement(
			$stmt, array(
				':locale' => $locale
			)
		);
		return $stmt->fetchAll();
	}

	static function InsertTranslation($repo, $locale, $user) {
		$user = (int)$user;

		$stmt = self::CreateStatement('INSERT INTO translations (repo, locale, user) VALUES (:repo, :locale, :user)');
		self::RunStatement(
			$stmt, array(
				':repo' => $repo,
				':locale' => $locale,
				':user' => $user
			)
		);
		return self::LastInsertId();
	}

	static function SetPullRequest($repo, $locale, $pullrequest) {
		$pullrequest = (int)$pullrequest;

		$stmt = self::CreateStatement('UPDATE translations SET pullrequest = :pullrequest WHERE repo = :repo AND locale = :locale');
		self::RunStatement(
			$stmt, array(
				':repo' => $repo,
				':locale' => $locale,
				':pullrequest' => $pullrequest
			)
		);
		return $stmt->rowCount() == 1;
	}

	static function DeleteTranslation($repo, $locale) {
		$stmt = self::CreateStatement('DELETE FROM translations WHERE repo = :repo AND locale = :locale');
		self::RunStatement(
			$stmt, array(
				':repo' => $repo,
				':locale' => $locale
			)
		);
		return $stmt->rowCount() == 1;
	}

	static function ArgumentList($functionName) {
		switch ($functionName) {
		case 'SelectTranslations':
			return array();
		case 'SelectOneTranslation':
			return array(
				'repo' => DBConnection::TYPE_STRING,
				'locale' => DBConnection::TYPE_STRING
			);
		case 'SelectTranslationsByUser':
			return array(
				'user' => DBConnection::TYPE_INTEGER
			);
		case 'SelectTranslationsByRepo':
			return array(
				'repo' => DBConnection::TYPE_STRING
			);
		case 'SelectTranslatorsByLocale':
			return array(
				'locale' => DBConnection::TYPE_STRING
			);
		case 'InsertTranslation':
			return array(
				'repo' => DBConnection::TYPE_STRING,
				'locale' => DBConnection::TYPE_STRING,
				'user' => DBConnection::TYPE_INTEGER
			);
		case 'SetPullRequest':
			return array(
				'repo' => DBConnection::TYPE_STRING,
				'locale' => DBConnection::TYPE_STRING,
				'pullrequest' => DBConnection::TYPE_INTEGER
			);
		case 'DeleteTranslation':
			return array(
				'repo' => DBConnection::TYPE_STRING,
				'locale' => DBConnection::TYPE_STRING
			);
		}
	}
}
