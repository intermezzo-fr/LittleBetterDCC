#ifndef DCC_AST_HPP
#define DCC_AST_HPP

#include "include/helper.hpp"
#include "include/ast/ast_fwd.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>

namespace dcc {
namespace ast {


// location
struct src_loc {
public:
  src_loc() : line(0), col(0) {}

  std::size_t line;
  std::size_t col;
};


// base
struct base {
public:
  base() noexcept {}
  virtual ~base() {}
  virtual std::string to_string() = 0;

  src_loc start_loc;
  src_loc end_loc;
};


// expression
struct expression : public base {
  expression() : base() {}
  virtual ~expression() {}
};


// statement
struct statement : public base {
  statement() {}
  virtual ~statement() {}
};


// constant
struct number_literal : public expression {
public:
  int value;

  explicit number_literal(int val) noexcept : value(val) {}
  ~number_literal() {}


  virtual std::string to_string() override {return "nunmber_literal : "  + std::to_string(value);}
private:
};



// identifier
struct identifier : public expression {
public:
  explicit identifier(std::string const &val) noexcept: expression(), value(val) {}
  ~identifier() {}
  virtual std::string to_string() override {return "identifier : " + value;}


  std::string value;
private:
};


// jump statement
struct jump_statement : public statement {
public:
  jump_statement(one_of_expr const &expr) : statement(), ret_expr(expr) {}
  ~jump_statement() {}
  virtual std::string to_string() override {return "jump_statement";}


  one_of_expr ret_expr;
private:
};


// call expression
struct call_expression : public expression {
public:
  explicit call_expression(one_of_expr const &func_name, std::vector<one_of_expr> const &args) noexcept
      : func_name(func_name), argument_list(args) {}
  ~call_expression() {}
  virtual std::string to_string() override {return "call_expression";}


  one_of_expr func_name;
  std::vector<one_of_expr> argument_list;
private:
};


// binary_expression
struct binary_expression : public expression {
public:
  explicit binary_expression(std::string const &op, one_of_expr const &lhs, one_of_expr const &rhs) noexcept
      : operand(op), lhs(lhs), rhs(rhs) {}
  ~binary_expression() {}
  virtual std::string to_string() override {return "binary_expression";}


  std::string operand;
  one_of_expr lhs;
  one_of_expr rhs;
private:
};


// variable declaration
struct variable_declaration : public statement {
public:
  explicit variable_declaration(std::string const &type, identifier_ptr const &name) noexcept : statement(), type(type), name(name) {}
  ~variable_declaration() {}
  virtual std::string to_string() override {return "variable_declaration";}


  std::string type;
  identifier_ptr name;
private:
};


// function statement
struct function_statement : public base{
public:
  function_statement(std::vector<one_of_statement> const &var_decl_list,
                     std::vector<one_of_statement> const &statements)
      : var_decls(var_decl_list), statements(statements) {}
  ~function_statement() {}
  virtual std::string to_string() override {return "function_statement";}


  // variable decl list
  std::vector<one_of_statement> var_decls;
  // statement list
  std::vector<one_of_statement> statements;
private:
};


// parameter
struct parameter : public base {
public:
  parameter(one_of_expr const &param) : param(param) {}
  ~parameter() {}

  virtual std::string to_string() override {return "parameter";}

  one_of_expr param;
private:
};


// prototype
struct prototype : public base{
public:
  prototype(identifier_ptr const &name, std::vector<one_of_expr> const &params) : func_name(name), param_list(params){}
  ~prototype() {}
  virtual std::string to_string() override {return "prototype : " + func_name->value + "(" + ")";}


  identifier_ptr func_name;
  std::vector<one_of_expr> param_list;
private:
};


// function definition
struct function_definition : public base{
public:
  function_definition(prototype_ptr const &sig, function_statement_ptr const &statements) : base(), signature(sig), statements(statements) {}
  ~function_definition() {}
  virtual std::string to_string() override {return "function_definition : " + signature->to_string();}


  prototype_ptr signature;
  function_statement_ptr statements;
private:
};


// function declaration
struct function_declaration : public base{
public:
  function_declaration(prototype_ptr const &sig) : base(), signature(sig) {}
  ~function_declaration() {}
  virtual std::string to_string() override {return "function_declaration" + signature->to_string();}


  prototype_ptr signature;
private:
};


// translation unit
struct translation_unit final : public base{
public:
  translation_unit(std::vector<function_declaration_ptr> const &decls, std::vector<function_definition_ptr> const &funcs) noexcept : 
    base(), func_decls(decls), functions(funcs) {}
  ~translation_unit() {}
  virtual std::string to_string() override {return "translation_unit";}


  std::vector<function_declaration_ptr> func_decls;
  std::vector<function_definition_ptr> functions;
private:
};


struct module {
public:
  module(translation_unit_ptr const &tunit, std::string const &name) : tunit(tunit), name(name){}
  translation_unit_ptr tunit;
  std::string name;
};


} // end of namespace ast
} // end of namespace dcc

#endif
