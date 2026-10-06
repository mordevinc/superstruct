use std::collections::HashMap;

pub type Flags = HashMap<String, String>;

pub fn parse_flags(args: &[String]) -> Flags {
    // args[0] - имя программы, начинаем с 1
    let mut flags = Flags::new();
    let mut i = 1;
    while i + 1 < args.len() {
        let arg = &args[i];
        if arg.starts_with("--") {
            flags.insert(arg.clone(), args[i + 1].clone());
            i += 2;
        } else {
            i += 1;
        }
    }
    flags
}

pub fn get_flag(flags: &Flags, key: &str) -> Option<String> {
    flags.get(key).cloned()
}