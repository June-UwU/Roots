#include "parser.hpp"
#include "assert.hpp"
#include "logger.hpp"

#define X(kind, name) name,

constexpr const char *ast_string[]{AST_TYPES AST_GENERAL};

#undef X

ast::ast(std::string id) : id(id) { kind = AST_MODULE; }

void ast::print_node(u32 nest_depth) {
    for (auto node : nodes) {
        node->print_node(nest_depth + 1);
    }
}

void ast::add_node(std::string_view &id, std::shared_ptr<ast_node> node) {
    module_nodes[id] = node;
}

std::shared_ptr<ast_node> ast::get_node(std::string_view &id) {
    if (module_nodes.end() == module_nodes.find(id)) {
        ASSERT(false, "diagnostics unknown function/ global look up");
        return std::shared_ptr<ast_node>();
    }

    return module_nodes[id];
}

ast_kind                  ast_node::get_type() { return kind; }

std::shared_ptr<ast_node> ast_node::get_parent() { return parent; }

variable_node::variable_node(ast_kind type, std::string_view &id,
                             std::shared_ptr<ast_node> root)
    : id(id), initialized(false) {
    kind   = type;
    parent = root;
}

variable_node::variable_node(ast_kind type, std::string_view &id,
                             std::shared_ptr<ast_node> root,
                             value_variant            &value)
    : id(id), value(value), initialized(true) {
    kind   = type;
    parent = root;
}

std::string_view             variable_node::get_id() { return id; }

variable_node::value_variant variable_node::get_value() {
    if (false == initialized) {
        ASSERT(false, "uninitialied variable value lookup");
    }
    return value;
}

void variable_node::print_node(u32 nest_depth) {
    u32         index       = GET_AST_INDEX(kind);
    const char *node_type   = ast_string[index];
    const char *init_status = "false";
    if (true == initialized) {
        init_status = "true";
    }

    log_message("%s[%s : %s ] %s", ANSI_COLOR_GREEN, node_type, init_status,
                ANSI_COLOR_RESET);
}

bool variable_node::is_initialized() { return initialized; }

function_node::function_node(std::string_view         &id,
                             std::shared_ptr<ast_node> root)
    : id(id), return_type(AST_INVALID) {
    kind   = AST_FUNCTION;
    parent = root;
}

bool     function_node::is_defined() { return (code.size() > 0); }

ast_kind function_node::get_return_type() {
    std::string id_string{id};
    ASSERT(return_type != AST_INVALID,
           "looking up return type for function %s whileout setting it",
           id_string.c_str());

    return return_type;
}

void function_node::print_declaration() {
    std::string id_string{id};
    log_message("%s %s(", ANSI_COLOR_MAGENTA, id_string.c_str());

    for (auto node : parameters) {
        node->print_node(0);
        log_message(", ");
    }

    u32 index = GET_AST_INDEX(return_type);
    log_message(") -> %s%s", ast_string[index], ANSI_COLOR_RESET);
}

void function_node::print_node(u32 nest_depth) {
    // id <[parameter list]> return_type
    // code nodes (+1 indendation)

    std::string id_string{id};
    log_message("%s %s(", ANSI_COLOR_MAGENTA, id_string.c_str());

    for (auto node : parameters) {
        node->print_node(0);
        log_message(", ");
    }

    u32 index = GET_AST_INDEX(return_type);
    log_message(") -> %s", ast_string[index]);

    for (auto node : code) {
        node->print_node(nest_depth + 1);
    }
}

void function_node::set_return_type(ast_kind type) { return_type = type; }

void function_node::add_code(std::shared_ptr<ast_node> code_section) {
    code.push_back(code_section);
}

void function_node::add_local(std::string_view          id,
                              std::shared_ptr<ast_node> local) {
    if (locals.end() != locals.find(id)) {
        ASSERT(false, "diagnostics local redeclaration");
    }

    locals[id] = local;
}

void function_node::add_parameters(
    std::vector<std::shared_ptr<ast_node>> &parameter_list) {
    parameters = parameter_list;
}

function_call_node::function_call_node(std::shared_ptr<ast_node> &root) {
    kind   = AST_FUNCTION_CALL;
    parent = root;
}

std::shared_ptr<ast_node> function_call_node::get_callee() {
    ASSERT(nullptr != callee.get(), "callee was never set");
    return callee;
}

std::shared_ptr<ast_node> function_call_node::get_caller() { return parent; }

void                      function_call_node::print_node(u32 nest_depth) {
    //
    function_node *callee_fn = reinterpret_cast<function_node *>(callee.get());
    callee_fn->print_declaration();
}

void function_call_node::set_callee(std::shared_ptr<ast_node> callee_node) {
    callee = callee_node;
}

void function_call_node::add_parameter(
    std::vector<std::shared_ptr<ast_node>> &parameter_list) {
    // TODO : function matching and callee setting
    parameters = parameter_list;
}
