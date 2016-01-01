/*******************************************************************\

Module: Variable Mapping

Author: Daniel Kroening, kroening@kroening.com

\*******************************************************************/

#ifndef CPROVER_TRANS_BV_VARID_H
#define CPROVER_TRANS_BV_VARID_H

#include <util/irep.h>

class bv_varidt
{
public:
  irep_idt id;
  unsigned bit_nr;
  enum class statet { CURRENT, NEXT } state;
  
  friend bool operator==(const bv_varidt &i1, const bv_varidt &i2)
  {
    return i1.id==i2.id && i1.bit_nr==i2.bit_nr;
  }
   
  friend bool operator<(const bv_varidt &i1, const bv_varidt &i2)
  {
    if(i1.id==i2.id)
      return i1.bit_nr<i2.bit_nr;

    return i1.id<i2.id;
  }
   
  inline bv_varidt(
    const irep_idt &_id,
    unsigned _bit_nr,
    statet _state):
    id(_id),
    bit_nr(_bit_nr),
    state(_state)
  { }
   
  inline bv_varidt():bit_nr(0), state(statet::CURRENT)
  { }
  
  std::string as_string() const;
};
 
struct bv_varidt_hash
{
  size_t operator()(const bv_varidt &bv_varid) const
  { return hash_string(bv_varid.id)^bv_varid.bit_nr^unsigned(bv_varid.state); }
};
 
#endif
