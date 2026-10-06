use crate::json_io::{self, JsonObject};
use crate::linear::{Array, ForwardList, DoublyList, Stack, Queue, Deque};

// тип структуры
#[derive(Clone, Copy, PartialEq, Eq, Debug)]
pub enum StructType {
    None,
    Array,
    Flist,
    Dlist,
    Stack,
    Queue,
    Deque,
}

// одна запись реестра
pub struct StructEntry {
    pub name: String,
    pub stype: StructType,
    pub array: Array,
    pub flist: ForwardList,
    pub dlist: DoublyList,
    pub stack: Stack,
    pub queue: Queue,
    pub deque: Deque,
}

impl StructEntry {
    // создаёт "пустую" запись со всеми структурами
    fn empty(name: &str, stype: StructType) -> Self {
        StructEntry {
            name: name.to_string(),
            stype,
            array: Array::new(100),
            flist: ForwardList::new(),
            dlist: DoublyList::new(),
            stack: Stack::new(),
            queue: Queue::new(),
            deque: Deque::new(),
        }
    }
}

// реестр всех структур
pub struct Registry {
    pub entries: Vec<StructEntry>,
}

impl Registry {
    // пустой реестр
    pub fn new() -> Self {
        Registry { entries: Vec::new() }
    }
}

// создать структуру
pub fn CreateStruct(reg: &mut Registry, name: &str, stype: StructType) {
    let e = StructEntry::empty(name, stype);
    reg.entries.push(e);
}

// найти по имени
pub fn FindStruct<'a>(reg: &'a mut Registry, name: &str) -> Option<&'a mut StructEntry> {
    reg.entries.iter_mut().find(|e| e.name == name)
}

// перевод типа в строку
pub fn TypeToString(stype: StructType) -> String {
    match stype {
        StructType::Array => "array".to_string(),
        StructType::Flist => "flist".to_string(),
        StructType::Dlist => "dlist".to_string(),
        StructType::Stack => "stack".to_string(),
        StructType::Queue => "queue".to_string(),
        StructType::Deque => "deque".to_string(),
        StructType::None => "none".to_string(),
    }
}

// перевод строки в тип
pub fn StringToType(s: &str) -> StructType {
    match s {
        "array" => StructType::Array,
        "flist" => StructType::Flist,
        "dlist" => StructType::Dlist,
        "stack" => StructType::Stack,
        "queue" => StructType::Queue,
        "deque" => StructType::Deque,
        _ => StructType::None,
    }
}

// сериализация одной структуры
fn serialize_entry(e: &StructEntry) -> String {
    match e.stype {
        StructType::Array => e.array.join_csv(),
        StructType::Flist => e.flist.join_csv(),
        StructType::Dlist => e.dlist.join_csv(),
        StructType::Stack => e.stack.join_csv(),
        StructType::Queue => e.queue.join_csv(),
        StructType::Deque => e.deque.join_csv(),
        StructType::None => String::new(),
    }
}

// сохранить реестр
pub fn SaveRegistry(reg: &Registry, filename: &str) {
    let mut obj = json_io::json_create();
    for e in &reg.entries {
        let key = format!("{}|{}", e.name, TypeToString(e.stype));
        json_io::json_set(&mut obj, &key, &serialize_entry(e));
    }
    json_io::json_write_to_file(&obj, filename);
}

// загрузить реестр
pub fn LoadRegistry(reg: &mut Registry, filename: &str) {
    let obj: JsonObject = json_io::json_read_from_file(filename);
    for (key, value) in obj.iter() {
        // ключ вида "myarray|array"
        let parts: Vec<&str> = key.split('|').collect();
        if parts.len() != 2 { continue; }
        let name = parts[0];
        let stype = StringToType(parts[1]);
        if stype == StructType::None { continue; }

        let mut entry = StructEntry::empty(name, stype);
        match stype {
            StructType::Array => entry.array = Array::from_csv(value),
            StructType::Flist => entry.flist = ForwardList::from_csv(value),
            StructType::Dlist => entry.dlist = DoublyList::from_csv(value),
            StructType::Stack => entry.stack = Stack::from_csv(value),
            StructType::Queue => entry.queue = Queue::from_csv(value),
            StructType::Deque => entry.deque = Deque::from_csv(value),
            StructType::None => {}
        }
        reg.entries.push(entry);
    }
}