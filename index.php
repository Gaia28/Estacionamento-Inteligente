<?php
session_start();

require_once 'controllers/AuthController.php';
require_once 'controllers/MonitorController.php';
require_once 'controllers/UserController.php';

// Define a rota padrão
$route = $_GET['route'] ?? 'monitor';

$authController = new AuthController();
$monitorController = new MonitorController();
$userController = new UserController();

switch ($route) {
    case 'login':
        $authController->handleLogin();
        break;
    case 'logout':
        $authController->handleLogout();
        break;
    case 'manage_users': 
        $userController->manageUsers();
        break;
    case 'monitor':
    default:
        $monitorController->showMonitor();
        break;
}
?>