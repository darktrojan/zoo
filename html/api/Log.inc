<?

class Log extends DBConnection {
	static function SelectLogItems() {
		$stmt = self::CreateStatement('SELECT id, UNIX_TIMESTAMP(created) AS created, user, action, repo, locale FROM log');
		self::RunStatement($stmt);
		return $stmt->fetchAll();
	}

	static function InsertLogItem($user, $action, $repo, $locale) {
		$user = (int)$user;

		$stmt = self::CreateStatement('INSERT INTO log (user, action, repo, locale) VALUES (:user, :action, :repo, :locale)');
		self::RunStatement(
			$stmt, array(
				':user' => $user,
				':action' => $action,
				':repo' => $repo,
				':locale' => $locale
			)
		);
		return self::LastInsertId();
	}

	static function DeleteLogItem($id) {
		$id = (int)$id;

		$stmt = self::CreateStatement('DELETE FROM log WHERE id = :id');
		self::RunStatement(
			$stmt, array(
				':id' => $id
			)
		);
		return $stmt->rowCount() == 1;
	}

	static function ArgumentList($functionName) {
		switch ($functionName) {
		case 'SelectLogItems':
			return array();
		case 'InsertLogItem':
			return array(
				'user' => DBConnection::TYPE_INTEGER,
				'action' => DBConnection::TYPE_STRING,
				'repo' => DBConnection::TYPE_STRING,
				'locale' => DBConnection::TYPE_STRING
			);
		case 'DeleteLogItem':
			return array(
				'id' => DBConnection::TYPE_INTEGER
			);
		}
	}
}
