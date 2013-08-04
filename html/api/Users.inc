<?

class Users extends DBConnection {
	static function SelectUsers() {
		$stmt = self::CreateStatement('SELECT id, email, name, github, locale FROM users');
		self::RunStatement($stmt);
		return $stmt->fetchAll();
	}

	static $cache = array();
	static function SelectOneUser($id) {
		$id = (int)$id;

		if (!isset(self::$cache[$id])) {
			$stmt = self::CreateStatement('SELECT email, name, github, locale FROM users WHERE id = :id');
			self::RunStatement(
				$stmt, array(
					':id' => $id
				)
			);
			self::$cache[$id] = $stmt->fetch() ?: null;
		}
		return self::$cache[$id];
	}

	static function SelectOneUserByEmail($email) {
		$stmt = self::CreateStatement('SELECT id, email, name, github, locale FROM users WHERE find_in_set(email, :email)');
		self::RunStatement(
			$stmt, array(
				':email' => $email
			)
		);
		return $stmt->fetch() ?: null;
	}

	static function InsertUser($email, $name, $github, $locale) {
		$stmt = self::CreateStatement('INSERT INTO users (email, name, github, locale) VALUES (:email, :name, :github, :locale)');
		self::RunStatement(
			$stmt, array(
				':email' => $email,
				':name' => $name,
				':github' => $github,
				':locale' => $locale
			)
		);
		return self::LastInsertId();
	}

	static function ReplaceUser($id, $email, $name, $github, $locale) {
		$id = (int)$id;

		$stmt = self::CreateStatement('REPLACE INTO users (id, email, name, github, locale) VALUES (:id, :email, :name, :github, :locale)');
		self::RunStatement(
			$stmt, array(
				':id' => $id,
				':email' => $email,
				':name' => $name,
				':github' => $github,
				':locale' => $locale
			)
		);
		return $stmt->rowCount() == 2;
	}

	static function UpdateUser($id, $email, $name, $github, $locale) {
		$id = (int)$id;

		$stmt = self::CreateStatement('UPDATE users SET email = :email, name = :name, github = :github, locale = :locale WHERE id = :id');
		self::RunStatement(
			$stmt, array(
				':id' => $id,
				':email' => $email,
				':name' => $name,
				':github' => $github,
				':locale' => $locale
			)
		);
		return $stmt->rowCount() == 1;
	}

	static function DeleteUser($id) {
		$id = (int)$id;

		$stmt = self::CreateStatement('DELETE FROM users WHERE id = :id');
		self::RunStatement(
			$stmt, array(
				':id' => $id
			)
		);
		return $stmt->rowCount() == 1;
	}

	static function ArgumentList($functionName) {
		switch ($functionName) {
		case 'SelectUsers':
			return array();
		case 'SelectOneUser':
			return array(
				'id' => DBConnection::TYPE_INTEGER
			);
		case 'SelectOneUserByEmail':
			return array(
				'email' => DBConnection::TYPE_STRING
			);
		case 'InsertUser':
			return array(
				'email' => DBConnection::TYPE_STRING,
				'name' => DBConnection::TYPE_STRING,
				'github' => DBConnection::TYPE_STRING,
				'locale' => DBConnection::TYPE_STRING
			);
		case 'ReplaceUser':
			return array(
				'id' => DBConnection::TYPE_INTEGER,
				'email' => DBConnection::TYPE_STRING,
				'name' => DBConnection::TYPE_STRING,
				'github' => DBConnection::TYPE_STRING,
				'locale' => DBConnection::TYPE_STRING
			);
		case 'UpdateUser':
			return array(
				'id' => DBConnection::TYPE_INTEGER,
				'email' => DBConnection::TYPE_STRING,
				'name' => DBConnection::TYPE_STRING,
				'github' => DBConnection::TYPE_STRING,
				'locale' => DBConnection::TYPE_STRING
			);
		case 'DeleteUser':
			return array(
				'id' => DBConnection::TYPE_INTEGER
			);
		}
	}
}
