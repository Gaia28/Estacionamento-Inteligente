<?php
require_once __DIR__ . '/../config/db.php';

class UserModel {
   
    public function authenticate($username, $password) {
        $db = getDbConnection();
        
        $stmt = $db->prepare("SELECT password FROM users WHERE username = ?");
        $stmt->execute([$username]);
        $user = $stmt->fetch(PDO::FETCH_ASSOC);

        if ($user && password_verify($password, $user['password'])) {
            return true;
        }
        return false;
    }
    public function registerUser($username, $password) {
        $db = getDbConnection();
        $password_hash = password_hash($password, PASSWORD_DEFAULT);

        try {
            $stmt = $db->prepare("INSERT INTO users (username, password) VALUES (?, ?)");
            return $stmt->execute([$username, $password_hash]);
        } catch (PDOException $e) {
            echo "usuário já cadastrado";
            return false;
        }
    }
}
?>