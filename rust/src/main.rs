mod json_io;
mod flags;
mod registry;
mod linear;

use std::env;
use registry::{Registry, StructType, CreateStruct, FindStruct, TypeToString, StringToType,
               SaveRegistry, LoadRegistry};

fn split_query(query: &str) -> Vec<String> {
    // делим строку по пробелам, пустые куски выбрасываем
    query.split_whitespace().map(|s| s.to_string()).collect()
}

fn handle_query(reg: &mut Registry, query: &str) {
    let t = split_query(query);
    if t.is_empty() {
        println!("empty query");
        return;
    }

    let cmd = t[0].as_str();

    // PRINT без имени печатает все структуры
    if cmd == "PRINT" {
        if t.len() < 2 {
            for e in reg.entries.iter() {
                println!("=== {} ({}) ===", e.name, TypeToString(e.stype));
                match e.stype {
                    StructType::Array => e.array.print(),
                    StructType::Flist => e.flist.print(),
                    StructType::Dlist => e.dlist.print(),
                    StructType::Stack => e.stack.print(),
                    StructType::Queue => e.queue.print(),
                    StructType::Deque => e.deque.print(),
                    StructType::None => {}
                }
            }
            return;
        }
        if let Some(e) = FindStruct(reg, &t[1]) {
            match e.stype {
                StructType::Array => e.array.print(),
                StructType::Flist => e.flist.print(),
                StructType::Dlist => e.dlist.print(),
                StructType::Stack => e.stack.print(),
                StructType::Queue => e.queue.print(),
                StructType::Deque => e.deque.print(),
                StructType::None => {}
            }
        } else {
            println!("not found");
        }
        return;
    }

    // CREATE <имя> <тип>
    if cmd == "CREATE" {
        if t.len() < 3 {
            println!("usage: CREATE <name> <type>");
            return;
        }
        let stype = StringToType(&t[2]);
        if stype == StructType::None {
            println!("unknown type");
            return;
        }
        if FindStruct(reg, &t[1]).is_some() {
            println!("already exists");
            return;
        }
        CreateStruct(reg, &t[1], stype);
        println!("-> created");
        return;
    }

    // МАССИВ 
    if cmd == "MPUSH" {
        if t.len() < 3 { println!("usage: MPUSH <name> <value>"); return; }
        if let Some(e) = FindStruct(reg, &t[1]) {
            if e.stype == StructType::Array {
                e.array.push(t[2].clone());
                println!("-> {}", t[2]);
            } else { println!("not found"); }
        } else { println!("not found"); }
    }
    else if cmd == "MPUSH_AT" {
        if t.len() < 4 { println!("usage: MPUSH_AT <name> <index> <value>"); return; }
        if let Some(e) = FindStruct(reg, &t[1]) {
            if e.stype == StructType::Array {
                let idx: usize = t[2].parse().unwrap_or(0);
                e.array.push_at(idx, t[3].clone());
                println!("-> {}", t[3]);
            } else { println!("not found"); }
        } else { println!("not found"); }
    }
    else if cmd == "MDEL" {
        if t.len() < 3 { println!("usage: MDEL <name> <index>"); return; }
        if let Some(e) = FindStruct(reg, &t[1]) {
            if e.stype == StructType::Array {
                let idx: usize = t[2].parse().unwrap_or(0);
                e.array.del(idx);
                println!("-> OK");
            } else { println!("not found"); }
        } else { println!("not found"); }
    }
    else if cmd == "MGET" {
        if t.len() < 3 { println!("usage: MGET <name> <index>"); return; }
        if let Some(e) = FindStruct(reg, &t[1]) {
            if e.stype == StructType::Array {
                let idx: usize = t[2].parse().unwrap_or(0);
                println!("-> {}", e.array.get(idx));
            } else { println!("not found"); }
        } else { println!("not found"); }
    }
    else if cmd == "MSET" {
        if t.len() < 4 { println!("usage: MSET <name> <index> <value>"); return; }
        if let Some(e) = FindStruct(reg, &t[1]) {
            if e.stype == StructType::Array {
                let idx: usize = t[2].parse().unwrap_or(0);
                e.array.set(idx, t[3].clone());
                println!("-> OK");
            } else { println!("not found"); }
        } else { println!("not found"); }
    }
    else if cmd == "MLEN" {
        if t.len() < 2 { println!("usage: MLEN <name>"); return; }
        if let Some(e) = FindStruct(reg, &t[1]) {
            if e.stype == StructType::Array {
                println!("-> {}", e.array.len());
            } else { println!("not found"); }
        } else { println!("not found"); }
    }

    // ОДНОСВЯЗНЫЙ 
    else if cmd == "FPUSH" {
        if t.len() < 3 { println!("usage: FPUSH <name> <value>"); return; }
        if let Some(e) = FindStruct(reg, &t[1]) {
            if e.stype == StructType::Flist {
                e.flist.push_back(t[2].clone());
                println!("-> {}", t[2]);
            } else { println!("not found"); }
        } else { println!("not found"); }
    }
    else if cmd == "FPUSH_FRONT" {
        if t.len() < 3 { println!("usage: FPUSH_FRONT <name> <value>"); return; }
        if let Some(e) = FindStruct(reg, &t[1]) {
            if e.stype == StructType::Flist {
                e.flist.push_front(t[2].clone());
                println!("-> {}", t[2]);
            } else { println!("not found"); }
        } else { println!("not found"); }
    }
    else if cmd == "FPUSH_AFTER" {
        if t.len() < 4 { println!("usage: FPUSH_AFTER <name> <anchor> <value>"); return; }
        if let Some(e) = FindStruct(reg, &t[1]) {
            if e.stype == StructType::Flist {
                if e.flist.push_after(&t[2], t[3].clone()) {
                    println!("-> {}", t[3]);
                } else {
                    println!("anchor not found");
                }
            } else { println!("not found"); }
        } else { println!("not found"); }
    }
    else if cmd == "FPUSH_BEFORE" {
        if t.len() < 4 { println!("usage: FPUSH_BEFORE <name> <anchor> <value>"); return; }
        if let Some(e) = FindStruct(reg, &t[1]) {
            if e.stype == StructType::Flist {
                if e.flist.push_before(&t[2], t[3].clone()) {
                    println!("-> {}", t[3]);
                } else {
                    println!("anchor not found");
                }
            } else { println!("not found"); }
        } else { println!("not found"); }
    }
    else if cmd == "FDEL" {
        if t.len() < 2 { println!("usage: FDEL <name>"); return; }
        if let Some(e) = FindStruct(reg, &t[1]) {
            if e.stype == StructType::Flist {
                e.flist.del_front();
                println!("-> OK");
            } else { println!("not found"); }
        } else { println!("not found"); }
    }
    else if cmd == "FDEL_BACK" {
        if t.len() < 2 { println!("usage: FDEL_BACK <name>"); return; }
        if let Some(e) = FindStruct(reg, &t[1]) {
            if e.stype == StructType::Flist {
                e.flist.del_back();
                println!("-> OK");
            } else { println!("not found"); }
        } else { println!("not found"); }
    }
    else if cmd == "FDEL_AFTER" {
        if t.len() < 3 { println!("usage: FDEL_AFTER <name> <anchor>"); return; }
        if let Some(e) = FindStruct(reg, &t[1]) {
            if e.stype == StructType::Flist {
                if e.flist.del_after(&t[2]) {
                    println!("-> OK");
                } else {
                    println!("anchor not found");
                }
            } else { println!("not found"); }
        } else { println!("not found"); }
    }
    else if cmd == "FDEL_BEFORE" {
        if t.len() < 3 { println!("usage: FDEL_BEFORE <name> <anchor>"); return; }
        if let Some(e) = FindStruct(reg, &t[1]) {
            if e.stype == StructType::Flist {
                if e.flist.del_before(&t[2]) {
                    println!("-> OK");
                } else {
                    println!("anchor not found");
                }
            } else { println!("not found"); }
        } else { println!("not found"); }
    }
    else if cmd == "FDEL_BY_VALUE" {
        if t.len() < 3 { println!("usage: FDEL_BY_VALUE <name> <value>"); return; }
        if let Some(e) = FindStruct(reg, &t[1]) {
            if e.stype == StructType::Flist {
                e.flist.del_by_value(&t[2]);
                println!("-> OK");
            } else { println!("not found"); }
        } else { println!("not found"); }
    }
    else if cmd == "FFIND" {
        if t.len() < 3 { println!("usage: FFIND <name> <value>"); return; }
        if let Some(e) = FindStruct(reg, &t[1]) {
            if e.stype == StructType::Flist {
                println!("-> {}", if e.flist.find(&t[2]) { "TRUE" } else { "FALSE" });
            } else { println!("not found"); }
        } else { println!("not found"); }
    }
    else if cmd == "FGET" {
        if t.len() < 2 { println!("usage: FGET <name>"); return; }
        if let Some(e) = FindStruct(reg, &t[1]) {
            if e.stype == StructType::Flist {
                e.flist.get_head();
            } else { println!("not found"); }
        } else { println!("not found"); }
    }
    else if cmd == "FGET_TAIL" {
        if t.len() < 2 { println!("usage: FGET_TAIL <name>"); return; }
        if let Some(e) = FindStruct(reg, &t[1]) {
            if e.stype == StructType::Flist {
                e.flist.get_tail();
            } else { println!("not found"); }
        } else { println!("not found"); }
    }
    else if cmd == "FGET_REVERSE" {
        if t.len() < 2 { println!("usage: FGET_REVERSE <name>"); return; }
        if let Some(e) = FindStruct(reg, &t[1]) {
            if e.stype == StructType::Flist {
                e.flist.get_reverse();
            } else { println!("not found"); }
        } else { println!("not found"); }
    }

    // ДВУСВЯЗНЫЙ 
    else if cmd == "LPUSH" {
        if t.len() < 3 { println!("usage: LPUSH <name> <value>"); return; }
        if let Some(e) = FindStruct(reg, &t[1]) {
            if e.stype == StructType::Dlist {
                e.dlist.push_back(t[2].clone());
                println!("-> {}", t[2]);
            } else { println!("not found"); }
        } else { println!("not found"); }
    }
    else if cmd == "LPUSH_FRONT" {
        if t.len() < 3 { println!("usage: LPUSH_FRONT <name> <value>"); return; }
        if let Some(e) = FindStruct(reg, &t[1]) {
            if e.stype == StructType::Dlist {
                e.dlist.push_front(t[2].clone());
                println!("-> {}", t[2]);
            } else { println!("not found"); }
        } else { println!("not found"); }
    }
    else if cmd == "LPUSH_AFTER" {
        if t.len() < 4 { println!("usage: LPUSH_AFTER <name> <anchor> <value>"); return; }
        if let Some(e) = FindStruct(reg, &t[1]) {
            if e.stype == StructType::Dlist {
                if e.dlist.push_after(&t[2], t[3].clone()) {
                    println!("-> {}", t[3]);
                } else {
                    println!("anchor not found");
                }
            } else { println!("not found"); }
        } else { println!("not found"); }
    }
    else if cmd == "LPUSH_BEFORE" {
        if t.len() < 4 { println!("usage: LPUSH_BEFORE <name> <anchor> <value>"); return; }
        if let Some(e) = FindStruct(reg, &t[1]) {
            if e.stype == StructType::Dlist {
                if e.dlist.push_before(&t[2], t[3].clone()) {
                    println!("-> {}", t[3]);
                } else {
                    println!("anchor not found");
                }
            } else { println!("not found"); }
        } else { println!("not found"); }
    }
    else if cmd == "LDEL" {
        if t.len() < 2 { println!("usage: LDEL <name>"); return; }
        if let Some(e) = FindStruct(reg, &t[1]) {
            if e.stype == StructType::Dlist {
                e.dlist.del_front();
                println!("-> OK");
            } else { println!("not found"); }
        } else { println!("not found"); }
    }
    else if cmd == "LDEL_BACK" {
        if t.len() < 2 { println!("usage: LDEL_BACK <name>"); return; }
        if let Some(e) = FindStruct(reg, &t[1]) {
            if e.stype == StructType::Dlist {
                e.dlist.del_back();
                println!("-> OK");
            } else { println!("not found"); }
        } else { println!("not found"); }
    }
    else if cmd == "LDEL_AFTER" {
        if t.len() < 3 { println!("usage: LDEL_AFTER <name> <anchor>"); return; }
        if let Some(e) = FindStruct(reg, &t[1]) {
            if e.stype == StructType::Dlist {
                if e.dlist.del_after(&t[2]) {
                    println!("-> OK");
                } else {
                    println!("anchor not found");
                }
            } else { println!("not found"); }
        } else { println!("not found"); }
    }
    else if cmd == "LDEL_BEFORE" {
        if t.len() < 3 { println!("usage: LDEL_BEFORE <name> <anchor>"); return; }
        if let Some(e) = FindStruct(reg, &t[1]) {
            if e.stype == StructType::Dlist {
                if e.dlist.del_before(&t[2]) {
                    println!("-> OK");
                } else {
                    println!("anchor not found");
                }
            } else { println!("not found"); }
        } else { println!("not found"); }
    }
    else if cmd == "LDEL_BY_VALUE" {
        if t.len() < 3 { println!("usage: LDEL_BY_VALUE <name> <value>"); return; }
        if let Some(e) = FindStruct(reg, &t[1]) {
            if e.stype == StructType::Dlist {
                e.dlist.del_by_value(&t[2]);
                println!("-> OK");
            } else { println!("not found"); }
        } else { println!("not found"); }
    }
    else if cmd == "LFIND" {
        if t.len() < 3 { println!("usage: LFIND <name> <value>"); return; }
        if let Some(e) = FindStruct(reg, &t[1]) {
            if e.stype == StructType::Dlist {
                println!("-> {}", if e.dlist.find(&t[2]) { "TRUE" } else { "FALSE" });
            } else { println!("not found"); }
        } else { println!("not found"); }
    }
    else if cmd == "LGET" {
        if t.len() < 2 { println!("usage: LGET <name>"); return; }
        if let Some(e) = FindStruct(reg, &t[1]) {
            if e.stype == StructType::Dlist {
                e.dlist.get_head();
            } else { println!("not found"); }
        } else { println!("not found"); }
    }
    else if cmd == "LGET_TAIL" {
        if t.len() < 2 { println!("usage: LGET_TAIL <name>"); return; }
        if let Some(e) = FindStruct(reg, &t[1]) {
            if e.stype == StructType::Dlist {
                e.dlist.get_tail();
            } else { println!("not found"); }
        } else { println!("not found"); }
    }
    else if cmd == "LGET_REVERSE" {
        if t.len() < 2 { println!("usage: LGET_REVERSE <name>"); return; }
        if let Some(e) = FindStruct(reg, &t[1]) {
            if e.stype == StructType::Dlist {
                e.dlist.get_reverse();
            } else { println!("not found"); }
        } else { println!("not found"); }
    }

    // СТЕК 
    else if cmd == "SPUSH" {
        if t.len() < 3 { println!("usage: SPUSH <name> <value>"); return; }
        if let Some(e) = FindStruct(reg, &t[1]) {
            if e.stype == StructType::Stack {
                e.stack.push(t[2].clone());
                println!("-> {}", t[2]);
            } else { println!("not found"); }
        } else { println!("not found"); }
    }
    else if cmd == "SPOP" {
        if t.len() < 2 { println!("usage: SPOP <name>"); return; }
        if let Some(e) = FindStruct(reg, &t[1]) {
            if e.stype == StructType::Stack {
                println!("-> {}", e.stack.pop());
            } else { println!("not found"); }
        } else { println!("not found"); }
    }

    // ОЧЕРЕДЬ 
    else if cmd == "QPUSH" {
        if t.len() < 3 { println!("usage: QPUSH <name> <value>"); return; }
        if let Some(e) = FindStruct(reg, &t[1]) {
            if e.stype == StructType::Queue {
                e.queue.push(t[2].clone());
                println!("-> {}", t[2]);
            } else { println!("not found"); }
        } else { println!("not found"); }
    }
    else if cmd == "QPOP" {
        if t.len() < 2 { println!("usage: QPOP <name>"); return; }
        if let Some(e) = FindStruct(reg, &t[1]) {
            if e.stype == StructType::Queue {
                println!("-> {}", e.queue.pop());
            } else { println!("not found"); }
        } else { println!("not found"); }
    }

    // DEQUE 
    else if cmd == "DPUSH_BACK" || cmd == "DPUSH" {
        if t.len() < 3 { println!("usage: DPUSH_BACK <name> <value>"); return; }
        if let Some(e) = FindStruct(reg, &t[1]) {
            if e.stype == StructType::Deque {
                e.deque.push_back(t[2].clone());
                println!("-> {}", t[2]);
            } else { println!("not found"); }
        } else { println!("not found"); }
    }
    else if cmd == "DPUSH_FRONT" {
        if t.len() < 3 { println!("usage: DPUSH_FRONT <name> <value>"); return; }
        if let Some(e) = FindStruct(reg, &t[1]) {
            if e.stype == StructType::Deque {
                e.deque.push_front(t[2].clone());
                println!("-> {}", t[2]);
            } else { println!("not found"); }
        } else { println!("not found"); }
    }
    else if cmd == "DPOP_FRONT" || cmd == "DPOP" {
        if t.len() < 2 { println!("usage: DPOP_FRONT <name>"); return; }
        if let Some(e) = FindStruct(reg, &t[1]) {
            if e.stype == StructType::Deque {
                println!("-> {}", e.deque.pop_front());
            } else { println!("not found"); }
        } else { println!("not found"); }
    }
    else if cmd == "DPOP_BACK" {
        if t.len() < 2 { println!("usage: DPOP_BACK <name>"); return; }
        if let Some(e) = FindStruct(reg, &t[1]) {
            if e.stype == StructType::Deque {
                println!("-> {}", e.deque.pop_back());
            } else { println!("not found"); }
        } else { println!("not found"); }
    }
    else if cmd == "DGET" || cmd == "DGET_HEAD" {
        if t.len() < 2 { println!("usage: DGET <name>"); return; }
        if let Some(e) = FindStruct(reg, &t[1]) {
            if e.stype == StructType::Deque {
                e.deque.get_head();
            } else { println!("not found"); }
        } else { println!("not found"); }
    }
    else if cmd == "DGET_TAIL" {
        if t.len() < 2 { println!("usage: DGET_TAIL <name>"); return; }
        if let Some(e) = FindStruct(reg, &t[1]) {
            if e.stype == StructType::Deque {
                e.deque.get_tail();
            } else { println!("not found"); }
        } else { println!("not found"); }
    }

    else {
        println!("unknown command: {}", cmd);
    }
}

fn main() {
    let args: Vec<String> = env::args().collect();

    // создаём папку data, если её нет
    std::fs::create_dir_all("data").ok();

    let flags = flags::parse_flags(&args);

    let mut filename = flags::get_flag(&flags, "--file")
        .unwrap_or_else(|| "data.json".to_string());
    if !filename.contains('/') && !filename.contains('\\') {
        filename = format!("data/{}", filename);
    }

    let mut reg = Registry::new();

    if std::path::Path::new(&filename).exists() {
        LoadRegistry(&mut reg, &filename);
    } else {
        CreateStruct(&mut reg, "myarray", StructType::Array);
        CreateStruct(&mut reg, "mylist", StructType::Flist);
        CreateStruct(&mut reg, "mydlist", StructType::Dlist);
        CreateStruct(&mut reg, "mystack", StructType::Stack);
        CreateStruct(&mut reg, "myqueue", StructType::Queue);
        CreateStruct(&mut reg, "mydeque", StructType::Deque);
        SaveRegistry(&reg, &filename);
    }

    let query = flags::get_flag(&flags, "--query").unwrap_or_default();
    if !query.is_empty() {
        handle_query(&mut reg, &query);
    } else {
        handle_query(&mut reg, "PRINT");
    }

    SaveRegistry(&reg, &filename);
}