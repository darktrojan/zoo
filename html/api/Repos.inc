<?

class Repos extends DBConnection {
	static function SelectRepos() {
		$stmt = self::CreateStatement('SELECT user, path, name, jetpack, locales, xpi_path, locale_path, branch FROM repos');
		self::RunStatement($stmt);
		return $stmt->fetchAll();
	}

	static function SelectReposByUser($user) {
		$stmt = self::CreateStatement('SELECT user, path, name, jetpack, locales, xpi_path, locale_path, branch FROM repos WHERE user = :user');
		self::RunStatement(
			$stmt, array(
				':user' => $user
			)
		);
		return $stmt->fetchAll();
	}

	static function SelectReposWithoutTranslation($locale) {
		$stmt = self::CreateStatement('SELECT user, path, name, jetpack, locales, xpi_path, locale_path, branch FROM repos WHERE NOT FIND_IN_SET(:locale, locales)');
		self::RunStatement(
			$stmt, array(
				':locale' => $locale
			)
		);
		return $stmt->fetchAll();
	}

	static function SelectOneRepo($path) {
		$stmt = self::CreateStatement('SELECT user, name, jetpack, locales, xpi_path, locale_path, branch FROM repos WHERE path = :path');
		self::RunStatement(
			$stmt, array(
				':path' => $path
			)
		);
		return $stmt->fetch() ?: null;
	}

	static function InsertRepo($user, $path, $name, $jetpack, $locales, $xpi_path, $locale_path, $branch) {
		$stmt = self::CreateStatement(
			'INSERT INTO repos (user, path, name, jetpack, locales, xpi_path, locale_path, branch) '.
			'VALUES (:user, :path, :name, :jetpack, :locales, :xpi_path, :locale_path, :branch)');
		self::RunStatement(
			$stmt, array(
				':user' => $user,
				':path' => $path,
				':name' => $name,
				':jetpack' => $jetpack,
				':locales' => $locales,
				':xpi_path' => $xpi_path,
				':locale_path' => $locale_path,
				':branch' => $branch
			)
		);
		return self::LastInsertId();
	}

	static function UpdateRepo($path, $name, $jetpack, $locales, $xpi_path, $locale_path, $branch) {
		$stmt = self::CreateStatement('UPDATE repos SET name = :name, jetpack = :jetpack, locales = :locales, xpi_path = :xpi_path, locale_path = :locale_path, branch = :branch WHERE path = :path');
		self::RunStatement(
			$stmt, array(
				':path' => $path,
				':name' => $name,
				':jetpack' => $jetpack,
				':locales' => $locales,
				':xpi_path' => $xpi_path,
				':locale_path' => $locale_path,
				':branch' => $branch
			)
		);
		return $stmt->rowCount() == 1;
	}

	static function UpdateRepoLocales($path, $locales) {
		$stmt = self::CreateStatement('UPDATE repos SET locales = :locales WHERE path = :path');
		self::RunStatement(
			$stmt, array(
				':path' => $path,
				':locales' => $locales
			)
		);
		return $stmt->rowCount() == 1;
	}

	static function DeleteRepo($path) {
		$stmt = self::CreateStatement('DELETE FROM repos WHERE path = :path');
		self::RunStatement(
			$stmt, array(
				':path' => $path
			)
		);
		return $stmt->rowCount() == 1;
	}

	static function ArgumentList($functionName) {
		switch ($functionName) {
		case 'SelectRepos':
			return array();
		case 'SelectReposByUser':
			return array(
				'user' => DBConnection::TYPE_INTEGER
			);
		case 'SelectReposWithoutTranslation':
			return array(
				'locale' => DBConnection::TYPE_STRING
			);
		case 'SelectOneRepo':
			return array(
				'path' => DBConnection::TYPE_STRING
			);
		case 'InsertRepo':
			return array(
				'user' => DBConnection::TYPE_INTEGER,
				'path' => DBConnection::TYPE_STRING,
				'name' => DBConnection::TYPE_STRING,
				'jetpack' => DBConnection::TYPE_STRING,
				'locales' => DBConnection::TYPE_STRING,
				'xpi_path' => DBConnection::TYPE_STRING,
				'locale_path' => DBConnection::TYPE_STRING,
				'branch' => DBConnection::TYPE_STRING
			);
		case 'UpdateRepo':
			return array(
				'path' => DBConnection::TYPE_STRING,
				'name' => DBConnection::TYPE_STRING,
				'jetpack' => DBConnection::TYPE_STRING,
				'locales' => DBConnection::TYPE_STRING,
				'xpi_path' => DBConnection::TYPE_STRING,
				'locale_path' => DBConnection::TYPE_STRING,
				'branch' => DBConnection::TYPE_STRING
			);
		case 'UpdateRepoLocales':
			return array(
				'path' => DBConnection::TYPE_STRING,
				'locales' => DBConnection::TYPE_STRING
			);
		case 'DeleteRepo':
			return array(
				'path' => DBConnection::TYPE_STRING
			);
		}
	}
}
