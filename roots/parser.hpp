#pragma once
#include "lexer.hpp"
#include "types.hpp"

#include <memory>
#include <string_view>
#include <unordered_map>
#include <variant>
#include <vector>

#define GET_AST_INDEX(type) (type - 1)

#define AST_ATTRIBUTES                                                         \
    X(INVALID, "invalid attribute")                                            \
    X(VAR_DECL_S8, "signed 8-bit integer")                                     \
    X(VAR_DECL_S16, "signed 16-bit integer")                                   \
    X(VAR_DECL_S32, "signed 32-bit integer")                                   \
    X(VAR_DECL_S64, "signed 64-bit integer")                                   \
    X(VAR_DECL_U8, "unsigned 8-bit integer")                                   \
    X(VAR_DECL_U16, "unsigned 16-bit integer")                                 \
    X(VAR_DECL_U32, "unsigned 32-bit integer")                                 \
    X(VAR_DECL_U64, "unsigned 64-bit integer")

#define AST_GENERAL                                                            \
    X(INVALID, "invalid")                                                      \
    X(FUNCTION, "function definition")                                         \
    X(FUNCTION_CALL, "function call")                                          \
    X(MODULE, "module")

#define X(kind, name) AST_##kind,

enum ast_kind { AST_KIND_BEGIN = 0x0, AST_GENERAL AST_KIND_END };

#undef X

#define X(attribute, name) AST_ATTRIBUTE_##attribute,

enum ast_attribute {
    AST_ATTRIBUTE_BEGIN = 0x0,
    AST_ATTRIBUTES AST_ATTRIBUTE_END
};

#undef X

class ast_node {
  public:
    ast_kind                  get_type();
    std::shared_ptr<ast_node> get_parent();
    void                      print_node(u32 nest_depth);

  protected:
    ast_kind                  kind;
    std::shared_ptr<ast_node> parent;
};

class variable_node : public ast_node {
  public:
    using value_variant =
        std::variant<s8, s16, s32, s64, u8, u16, u32, u64, std::string_view>;
    variable_node(ast_kind kind, std::string_view &id,
                  std::shared_ptr<ast_node> parent);
    variable_node(ast_kind kind, std::string_view &id,
                  std::shared_ptr<ast_node> parent, value_variant &value);

    std::string_view get_id();
    value_variant    get_value();
    bool             is_initialized();
    void             print_node(u32 nest_depth);

  private:
    std::string_view id;
    ast_attribute    type;
    value_variant    value;
    bool             initialized;
};

class function_node : public ast_node {
  public:
    function_node(std::string_view &id, std::shared_ptr<ast_node> parent);

    bool          is_defined();
    ast_attribute get_return_type();
    void          print_declaration();
    void          print_node(u32 nest_depth);
    void          set_return_type(ast_attribute type);
    void          add_code(std::shared_ptr<ast_node> code_section);
    void add_local(std::string_view id, std::shared_ptr<ast_node> local);
    void add_parameters(std::vector<std::shared_ptr<ast_node>> &parameter_list);

  private:
    std::string_view                                                id;
    std::vector<std::shared_ptr<ast_node>>                          code;
    std::unordered_map<std::string_view, std::shared_ptr<ast_node>> locals;
    std::vector<std::shared_ptr<ast_node>>                          parameters;
    ast_attribute                                                   return_type;
};

class function_call_node : ast_node {
  public:
    function_call_node(std::string_view &id,std::shared_ptr<ast_node> &parent);

    std::shared_ptr<ast_node> get_callee();
    std::shared_ptr<ast_node> get_caller();
    void                      print_node(u32 nest_depth);
    void                      set_callee(std::shared_ptr<ast_node> callee);
    void add_parameter(std::vector<std::shared_ptr<ast_node>> &parameter_list);

  private:
    std::string_view                       id;
    std::shared_ptr<ast_node>              callee;
    std::vector<std::shared_ptr<ast_node>> parameters;
};

class ast : ast_node {
  public:
    ast(std::string id);

    void                      print_node(u32 nest_depth);
    std::shared_ptr<ast_node> get_node(std::string_view &id);
    void add_node(std::string_view &id, std::shared_ptr<ast_node> node);

  private:
    std::string                            id;
    std::vector<std::shared_ptr<ast_node>> nodes;
    std::unordered_map<std::string_view, std::shared_ptr<ast_node>>
        module_nodes;
};

std::shared_ptr<ast_node> parse_module(std::string        &id,
                                       std::vector<token> &token_list);