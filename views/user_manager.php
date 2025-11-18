<!DOCTYPE html>
<html lang="pt-BR">
<head>
    <meta charset="UTF-8">
    <title>Gerenciamento de Usuários (Admin)</title>
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <link rel="stylesheet" href="assets/style.css">
    <style>
        /* Estilos específicos para a tela de admin */
        .admin-container {
            background-color: #ffffff;
            padding: 30px;
            border-radius: 8px;
            box-shadow: 0 4px 10px rgba(0, 0, 0, 0.1);
            width: 100%;
            max-width: 450px;
            text-align: left;
            margin-top: 30px;
        }
        .admin-container h2 {
            color: #17a2b8; /* Cor de destaque para Admin */
            border-bottom: 2px solid #17a2b8;
            padding-bottom: 10px;
            margin-bottom: 20px;
        }
        .message-success {
            color: #28a745;
            font-weight: bold;
            margin-bottom: 15px;
        }
        .message-error {
            color: #dc3545;
            font-weight: bold;
            margin-bottom: 15px;
        }
        /* Reutilizando estilos de input e botão do login */
        .admin-container input[type="text"], .admin-container input[type="password"] {
            width: 100%;
            padding: 10px;
            margin: 10px 0;
            border: 1px solid #ccc;
            border-radius: 4px;
            box-sizing: border-box;
        }
        .admin-container button {
            background-color: #17a2b8;
            margin-top: 15px;
        }
        .admin-container button:hover {
            background-color: #138496;
        }
    </style>
</head>
<body>
    <a href="?route=monitor" style="margin-bottom: 20px; display: block; color: #007bff; text-decoration: none;">&larr; Voltar ao Monitoramento</a>

    <div class="admin-container">
        <h2>Cadastro de Novo Funcionário</h2>

        <?php if (!empty($message)): ?>
            <div class="message-success"><?php echo htmlspecialchars($message); ?></div>
        <?php endif; ?>
        <?php if (!empty($error)): ?>
            <div class="message-error"><?php echo htmlspecialchars($error); ?></div>
        <?php endif; ?>
        
        <form method="POST" action="?route=manage_users">
            <label for="new_username">Nome de Usuário:</label>
            <input type="text" id="new_username" name="new_username" required>
            
            <label for="new_password">Senha:</label>
            <input type="password" id="new_password" name="new_password" required>
            
            <button type="submit">Cadastrar Funcionário</button>
        </form>
    </div>
</body>
</html>