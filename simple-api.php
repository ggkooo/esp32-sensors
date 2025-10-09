<?php

header('Content-Type: application/json');

$log_file = 'sensor_data_history.json';

$required_fields = ['temperature', 'humidity', 'noise', 'pression', 'eco2', 'tvoc', 'timestamp'];

if ($_SERVER['REQUEST_METHOD'] !== 'POST') {
    http_response_code(405);
    echo json_encode(['status' => 'error', 'message' => 'Método de requisição não permitido. Use POST.']);
    exit;
}

$json_data = file_get_contents('php://input');
$new_data_raw = json_decode($json_data, true);

if (empty($new_data_raw)) {
    http_response_code(400);
    echo json_encode(['status' => 'error', 'message' => 'Dados JSON inválidos ou ausentes.']);
    exit;
}

if (!is_array($new_data_raw)) {
    $new_data_raw = [$new_data_raw];
}

$valid_records_to_save = [];

foreach ($new_data_raw as $record) {
    $isValid = true;

    foreach ($required_fields as $field) {
        if (!isset($record[$field])) {
            $isValid = false;
            break;
        }
    }

    if ($isValid) {
        $valid_records_to_save[] = $record;
    }
}

if (empty($valid_records_to_save)) {
    http_response_code(400);
    echo json_encode(['status' => 'error', 'message' => 'Nenhum registro válido encontrado para salvar.']);
    exit;
}

$existing_records = [];

if (file_exists($log_file)) {
    $existing_content = file_get_contents($log_file);

    if (!empty($existing_content)) {
        $existing_records = json_decode($existing_content, true);

        if (!is_array($existing_records)) {
            $existing_records = [];
        }
    }
}

$all_records = array_merge($existing_records, $valid_records_to_save);

$final_json = json_encode($all_records, JSON_PRETTY_PRINT | JSON_UNESCAPED_SLASHES | JSON_UNESCAPED_UNICODE);

if ($final_json === false || file_put_contents($log_file, $final_json) === false) {
    http_response_code(500);
    echo json_encode(['status' => 'error', 'message' => 'Não foi possível escrever no arquivo de log. Verifique as permissões de pasta (chmod 777).']);
    exit;
}

http_response_code(200);
echo json_encode([
    'status' => 'success',
    'message' => count($valid_records_to_save) . ' registros salvos com sucesso.',
    'total_records' => count($all_records)
]);
