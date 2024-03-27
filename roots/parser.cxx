#include "parser.hpp"
#include "assert.hpp"
#include "logger.hpp"

#define X(kind, name) name,

constexpr const char *ast_string[]{AST_GENERAL};
constexpr const char *ast_attribute_string[]{AST_ATTRIBUTES};

#undef X

ast::ast(std::string id)
    : id(id) {
    kind = AST_MODULE;
}

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
        std::string look_up(id);
        ASSERT(false, "diagnostics undeclared symbol : %s ",look_up.c_str());
        return std::shared_ptr<ast_node>();
    }

    return module_nodes[id];
}

ast_kind                  ast_node::get_type() { return kind; }

std::shared_ptr<ast_node> ast_node::get_parent() { return parent; }

void                      ast_node::print_node(u32 nest_depth) {}

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
        std::string symbol_name(id);
        ASSERT(false, "diagnostic (warning) uninitialied variable : %s",symbol_name.c_str());
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
    : id(id), return_type(AST_ATTRIBUTE_INVALID) {
    kind   = AST_FUNCTION;
    parent = root;
}

bool          function_node::is_defined() { return (code.size() > 0); }

ast_attribute function_node::get_return_type() {
    std::string id_string{id};
    ASSERT(return_type != AST_ATTRIBUTE_INVALID,
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
    log_message(") -> %s%s", ast_attribute_string[index], ANSI_COLOR_RESET);
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
    log_message(") -> %s", ast_attribute_string[index]);

    for (auto node : code) {
        node->print_node(nest_depth + 1);
    }
}

void function_node::set_return_type(ast_attribute type) { return_type = type; }

void function_node::add_code(std::shared_ptr<ast_node> code_section) {
    code.push_back(code_section);
}

void function_node::add_local(std::string_view          id,
                              std::shared_ptr<ast_node> local) {
    if (locals.end() != locals.find(id)) {
        std::string symbol_name(id);
        ASSERT(false, "diagnostics error local redeclaration : %s", symbol_name.c_str());
    }

    locals[id] = local;
}

void function_node::add_parameters(
    std::vector<std::shared_ptr<ast_node>> &parameter_list) {
    parameters = parameter_list;
}

function_call_node::function_call_node(std::string_view &id,std::shared_ptr<ast_node> &root) 
    : id(id){
    kind   = AST_FUNCTION_CALL;
    parent = root;
}

std::shared_ptr<ast_node> function_call_node::get_callee() {
    std::string symbol_name(id);
    ASSERT(nullptr != callee.get(), "callee was never set %s",symbol_name.c_str());
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

// class parser_context {
//   public:
//     parser_context(u32 iter, std::vector<token> &token_list)
//         : iter(iter), tokens(token_list){};

//     bool is_module_end() {
//         ASSERT(iter < tokens.size(), "diagnostic out of index token look up : %d",iter);
//         return SOURCE_EOF == get_current_kind();
//     }

//     token_kind get_current_kind() { return tokens[iter].get_kind(); }
//     void       consume_token() {
//         iter = iter + 1;
//         ASSERT(iter < tokens.size(), "diagnostics reached the source end while parsing, maybe print token and position %d",iter);
//     }

//     std::shared_ptr<ast_node> parse_function(std::shared_ptr<ast_node> parent) {
//         return std::shared_ptr<ast_node>();
//     }

//   private:
//     u32                 iter;
//     std::vector<token>  tokens;
// };

std::shared_ptr<ast_node> parse_module(std::string        &id,
                                       std::vector<token> &token_list) {

    ast_node *raw_node = reinterpret_cast<ast_node *>(new ast(id));
    std::shared_ptr<ast_node> module_node(raw_node);

    // parser_context            parser(0, token_list);
    // while (false == parser.is_module_end()) {

    //     token_kind current_kind = parser.get_current_kind();
    //     switch (current_kind) {
    //     case FUNCTION: {
    //         std::shared_ptr<ast_node> fn_node =
    //             parser.parse_function(module_node);
    //     } break;
    //     case SOURCE_EOF: {
    //         ASSERT(false, "early exit for tokens case shouldn't happen %s",id.c_str());
    //     } break;
    //     case IDENTIFIER: {

    //     } break;
    //     }
    // }

    return module_node;
}
