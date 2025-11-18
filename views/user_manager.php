<!DOCTYPE html>
<html lang="pt-BR">
<head>
    <meta charset="UTF-8">
    <title>Gerenciamento de Usuários (Admin)</title>
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <link rel="stylesheet" href="assets/style.css"> 
    <link rel="stylesheet" href="assets/header.css">
    <link rel="stylesheet" href="assets/pageAdmin.css"> 

    </head>
<body>
    
    <?php 
    include 'views/header.php'; 
    ?>

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