/*******************************************************************\

Module: Negation Normal Form for temporal logic

Author: Daniel Kroening, dkr@amazon.com

\*******************************************************************/

#include "nnf.h"

#include <verilog/sva_expr.h>

#include "temporal_expr.h"

std::optional<exprt> negate_property_node(const exprt &expr)
{
  if(expr.id() == ID_U)
  {
    // ¬(φ U ψ) ≡ (¬φ R ¬ψ)
    auto &U = to_U_expr(expr);
    return R_exprt{not_exprt{U.lhs()}, not_exprt{U.rhs()}};
  }
  else if(expr.id() == ID_R)
  {
    // ¬(φ R ψ) ≡ (¬φ U ¬ψ)
    auto &R = to_R_expr(expr);
    return U_exprt{not_exprt{R.lhs()}, not_exprt{R.rhs()}};
  }
  else if(expr.id() == ID_G)
  {
    // ¬G φ ≡ F ¬φ
    auto &G = to_G_expr(expr);
    return F_exprt{not_exprt{G.op()}};
  }
  else if(expr.id() == ID_F)
  {
    // ¬F φ ≡ G ¬φ
    auto &F = to_F_expr(expr);
    return G_exprt{not_exprt{F.op()}};
  }
  else if(expr.id() == ID_X)
  {
    // ¬X φ ≡ X ¬φ
    auto &X = to_X_expr(expr);
    return X_exprt{not_exprt{X.op()}};
  }
  else if(expr.id() == ID_implies)
  {
    // ¬(a->b) ≡ a && ¬b
    auto &implies_expr = to_implies_expr(expr);
    return and_exprt{implies_expr.lhs(), not_exprt{implies_expr.rhs()}};
  }
  else if(expr.id() == ID_and)
  {
    auto operands = expr.operands();
    for(auto &op : operands)
      op = not_exprt{op};
    return or_exprt{std::move(operands)};
  }
  else if(expr.id() == ID_or)
  {
    auto operands = expr.operands();
    for(auto &op : operands)
      op = not_exprt{op};
    return and_exprt{std::move(operands)};
  }
  else if(expr.id() == ID_not)
  {
    return to_not_expr(expr).op();
  }
  else if(expr.id() == ID_sva_until)
  {
    // ¬(φ W ψ) ≡ (¬φ strongR ¬ψ)
    auto &W = to_sva_until_expr(expr);
    return strong_R_exprt{not_exprt{W.lhs()}, not_exprt{W.rhs()}};
  }
  else if(expr.id() == ID_sva_s_until)
  {
    // ¬(φ U ψ) ≡ (¬φ R ¬ψ)
    auto &U = to_sva_s_until_expr(expr);
    return R_exprt{not_exprt{U.lhs()}, not_exprt{U.rhs()}};
  }
  else if(expr.id() == ID_sva_until_with)
  {
    // ¬(φ R ψ) ≡ (¬φ U ¬ψ)
    // Note LHS and RHS are swapped.
    auto &until_with = to_sva_until_with_expr(expr);
    auto R = R_exprt{until_with.rhs(), until_with.lhs()};
    return sva_until_exprt{not_exprt{R.lhs()}, not_exprt{R.rhs()}};
  }
  else if(expr.id() == ID_sva_s_until_with)
  {
    // ¬(φ strongR ψ) ≡ (¬φ W ¬ψ)
    // Note LHS and RHS are swapped.
    auto &s_until_with = to_sva_s_until_with_expr(expr);
    auto strong_R = strong_R_exprt{s_until_with.rhs(), s_until_with.lhs()};
    return weak_U_exprt{not_exprt{strong_R.lhs()}, not_exprt{strong_R.rhs()}};
  }
  else
    return {};
}
