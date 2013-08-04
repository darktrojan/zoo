<?

class Files extends DBConnection {
	static function SelectFiles() {
		$stmt = self::CreateStatement('SELECT repo, locale, file, UNIX_TIMESTAMP(updated) AS updated, string_count, string_dupe FROM files');
		self::RunStatement($stmt);
		return $stmt->fetchAll();
	}

	static function SelectFilesByLocale($repo, $locale) {
		$stmt = self::CreateStatement('SELECT file, UNIX_TIMESTAMP(updated) AS updated, string_count, string_dupe FROM files WHERE repo = :repo AND locale = :locale');
		self::RunStatement(
			$stmt, array(
				':repo' => $repo,
				':locale' => $locale
			)
		);
		return $stmt->fetchAll();
	}

	static function SelectSummaryAll() {
		$stmt = self::CreateStatement('SELECT locale, SUM(string_count) AS string_count FROM `files` GROUP BY locale ORDER BY string_count DESC');
		self::RunStatement($stmt);
		return $stmt->fetchAll();
	}

	static function SelectSummary($repo) {
		$stmt = self::CreateStatement('SELECT locale, UNIX_TIMESTAMP(MAX(updated)) AS updated, SUM(string_count) AS string_count, SUM(string_dupe) AS string_dupe FROM `files` WHERE repo = :repo GROUP BY locale');
		self::RunStatement(
			$stmt, array(
				':repo' => $repo
			)
		);
		return $stmt->fetchAll();
	}

	static function SelectOneFile($repo, $locale, $file) {
		$stmt = self::CreateStatement('SELECT UNIX_TIMESTAMP(updated) AS updated, string_count, string_dupe FROM files WHERE repo = :repo AND locale = :locale AND file = :file');
		self::RunStatement(
			$stmt, array(
				':repo' => $repo,
				':locale' => $locale,
				':file' => $file
			)
		);
		return $stmt->fetch() ?: null;
	}

	static function InsertFile($repo, $locale, $file, $string_count, $string_dupe) {
		$string_count = (int)$string_count;
		$string_dupe = (int)$string_dupe;

		$stmt = self::CreateStatement('INSERT INTO files (repo, locale, file, string_count, string_dupe) VALUES (:repo, :locale, :file, :string_count, :string_dupe)');
		self::RunStatement(
			$stmt, array(
				':repo' => $repo,
				':locale' => $locale,
				':file' => $file,
				':string_count' => $string_count,
				':string_dupe' => $string_dupe
			)
		);
		return self::LastInsertId();
	}

	static function CreateLocale($repo, $locale) {
		$stmt = self::CreateStatement('INSERT INTO files (SELECT repo, :locale, file, NOW() , string_count, string_count FROM files WHERE repo = :repo AND locale = \'en-US\')');
		self::RunStatement(
			$stmt, array(
				':repo' => $repo,
				':locale' => $locale
			)
		);
		return $stmt->rowCount();
	}

	static function ReplaceFile($repo, $locale, $file, $string_count, $string_dupe) {
		$updated = (int)$updated;
		$string_count = (int)$string_count;
		$string_dupe = (int)$string_dupe;

		$stmt = self::CreateStatement('REPLACE INTO files (repo, locale, file, updated, string_count, string_dupe) VALUES (:repo, :locale, :file, NOW(), :string_count, :string_dupe)');
		self::RunStatement(
			$stmt, array(
				':repo' => $repo,
				':locale' => $locale,
				':file' => $file,
				':string_count' => $string_count,
				':string_dupe' => $string_dupe
			)
		);
		return $stmt->rowCount() == 2;
	}

	static function UpdateFile($repo, $locale, $file, $updated, $string_count, $string_dupe) {
		$updated = (int)$updated;
		$string_count = (int)$string_count;
		$string_dupe = (int)$string_dupe;

		$stmt = self::CreateStatement('UPDATE files SET updated = FROM_UNIXTIME(:updated), string_count = :string_count, string_dupe = :string_dupe WHERE repo = :repo AND locale = :locale AND file = :file');
		self::RunStatement(
			$stmt, array(
				':repo' => $repo,
				':locale' => $locale,
				':file' => $file,
				':updated' => $updated,
				':string_count' => $string_count,
				':string_dupe' => $string_dupe
			)
		);
		return $stmt->rowCount() == 1;
	}

	static function DeleteFile($repo, $locale, $file) {
		$stmt = self::CreateStatement('DELETE FROM files WHERE repo = :repo AND locale = :locale AND file = :file');
		self::RunStatement(
			$stmt, array(
				':repo' => $repo,
				':locale' => $locale,
				':file' => $file
			)
		);
		return $stmt->rowCount() == 1;
	}

	static function ArgumentList($functionName) {
		switch ($functionName) {
		case 'SelectFiles':
			return array();
		case 'SelectFilesByLocale':
			return array(
				'repo' => DBConnection::TYPE_STRING,
				'locale' => DBConnection::TYPE_STRING
			);
		case 'SelectSummaryAll':
			return array();
		case 'SelectSummary':
			return array(
				'repo' => DBConnection::TYPE_STRING
			);
		case 'SelectOneFile':
			return array(
				'repo' => DBConnection::TYPE_STRING,
				'locale' => DBConnection::TYPE_STRING,
				'file' => DBConnection::TYPE_STRING
			);
		case 'InsertFile':
			return array(
				'repo' => DBConnection::TYPE_STRING,
				'locale' => DBConnection::TYPE_STRING,
				'file' => DBConnection::TYPE_STRING,
				'string_count' => DBConnection::TYPE_INTEGER,
				'string_dupe' => DBConnection::TYPE_INTEGER
			);
		case 'CreateLocale':
			return array(
				'repo' => DBConnection::TYPE_STRING,
				'locale' => DBConnection::TYPE_STRING
			);
		case 'ReplaceFile':
			return array(
				'repo' => DBConnection::TYPE_STRING,
				'locale' => DBConnection::TYPE_STRING,
				'file' => DBConnection::TYPE_STRING,
				'string_count' => DBConnection::TYPE_INTEGER,
				'string_dupe' => DBConnection::TYPE_INTEGER
			);
		case 'UpdateFile':
			return array(
				'repo' => DBConnection::TYPE_STRING,
				'locale' => DBConnection::TYPE_STRING,
				'file' => DBConnection::TYPE_STRING,
				'updated' => DBConnection::TYPE_TIMESTAMP,
				'string_count' => DBConnection::TYPE_INTEGER,
				'string_dupe' => DBConnection::TYPE_INTEGER
			);
		case 'DeleteFile':
			return array(
				'repo' => DBConnection::TYPE_STRING,
				'locale' => DBConnection::TYPE_STRING,
				'file' => DBConnection::TYPE_STRING
			);
		}
	}
}
