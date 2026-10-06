use std::collections::HashMap;
use std::fs;

// плоский json: ключ → значение, оба строки
pub type JsonObject = HashMap<String, String>;

pub fn json_create() -> JsonObject {
    JsonObject::new()
}

pub fn json_set(obj: &mut JsonObject, key: &str, value: &str) {
    obj.insert(key.to_string(), value.to_string());
}

pub fn json_get(obj: &JsonObject, key: &str) -> String {
    obj.get(key).cloned().unwrap_or_default()
}

pub fn json_to_string(obj: &JsonObject) -> String {
    let mut result = String::from("{\n");
    let n = obj.len();
    for (i, (k, v)) in obj.iter().enumerate() {
        result.push_str(&format!("  \"{}\": \"{}\"", k, v));
        if i != n - 1 {
            result.push(',');
        }
        result.push('\n');
    }
    result.push('}');
    result
}

pub fn json_parse(text: &str) -> JsonObject {
    let mut obj = JsonObject::new();
    let chars: Vec<char> = text.chars().collect();
    let mut i = 0;

    while i < chars.len() {
        // ищем открывающую кавычку ключа
        while i < chars.len() && chars[i] != '"' { i += 1; }
        if i >= chars.len() { break; }
        i += 1; // пропускаем "

        // читаем ключ
        let mut key = String::new();
        while i < chars.len() && chars[i] != '"' {
            key.push(chars[i]);
            i += 1;
        }
        i += 1; // пропускаем "

        // ищем открывающую кавычку значения
        while i < chars.len() && chars[i] != '"' { i += 1; }
        if i >= chars.len() { break; }
        i += 1; // пропускаем "

        // читаем значение
        let mut value = String::new();
        while i < chars.len() && chars[i] != '"' {
            value.push(chars[i]);
            i += 1;
        }
        i += 1; // пропускаем "

        obj.insert(key, value);
    }

    obj
}

pub fn json_write_to_file(obj: &JsonObject, filename: &str) {
    if let Err(e) = fs::write(filename, json_to_string(obj)) {
        println!("Cannot write file {}: {}", filename, e);
    }
}

pub fn json_read_from_file(filename: &str) -> JsonObject {
    match fs::read_to_string(filename) {
        Ok(text) => json_parse(&text),
        Err(e) => {
            println!("Cannot read file {}: {}", filename, e);
            json_create()
        }
    }
}