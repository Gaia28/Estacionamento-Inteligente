<?php

require_once __DIR__ . '/../models/UserModel.php';

class UserController {
    private $userModel;

    public function __construct() {
        $this->userModel = new UserModel();
    }

    public function manageUsers() {
        if (!isset($_SESSION['logged_in']) || $_SESSION['logged_in'] !== true || $_SESSION['username'] !== 'admin') {
            header('Location: ?route=monitor'); 
            exit;
        }

        $message = '';
        $error = '';

        if ($_SERVER['REQUEST_METHOD'] === 'POST') {
            $newUsername = trim($_POST['new_username'] ?? '');
            $newPassword = $_POST['new_password'] ?? '';

            if (empty($newUsername) || empty($newPassword)) {
                $error = 'Preencha o nome de usuário e a senha.';
            } else {
                if ($this->userModel->registerUser($newUsername, $newPassword)) {
                    $message = "Usuário '{$newUsername}' cadastrado com sucesso!";
                } else {
                    $error = "Erro ao cadastrar. O usuário '{$newUsername}' provavelmente já existe.";
                }
            }
        }
        
        $admin_username = $_SESSION['username'];
        require __DIR__ . '/../views/user_manager.php';
    }
}
?>