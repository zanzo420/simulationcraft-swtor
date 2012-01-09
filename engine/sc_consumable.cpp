// ==========================================================================
// Dedmonwakeen's Raid DPS/TPS Simulator.
// Send questions to natehieter@gmail.com
// ==========================================================================

#include "simulationcraft.h"

// ==========================================================================
// Consumable
// ==========================================================================


struct stim_t : public action_t
{
  int type;
  gain_t* gain;

  stim_t( player_t* p, const std::string& options_str ) :
    action_t( ACTION_USE, "stim", p ), type( STIM_NONE )
  {
    std::string type_str;

    option_t options[] =
    {
      { "type", OPT_STRING, &type_str },
      { NULL, OPT_UNKNOWN, NULL }
    };
    parse_options( options, options_str );

    trigger_gcd = 0;
    harmful = false;
    for ( int i=0; i < STIM_MAX; i++ )
    {
      if ( type_str == util_t::stim_type_string( i ) )
      {
        type = i;
        break;
      }
    }
    assert( type != STIM_NONE );
    gain = p -> get_gain( "stim" );
  }

  virtual void execute()
  {
    player_t* p = player;
    if ( sim -> log ) log_t::output( sim, "%s uses Stim %s", p -> name(), util_t::stim_type_string( type ) );
    p -> stim = type;
    double willpower = 0, power = 0;
    switch ( type )
    {

    case STIM_RAKATA_RESOLVE:
      willpower = 136.0; power = 56;
      p -> stat_gain( STAT_WILLPOWER, willpower, gain, this );
      p -> stat_gain( STAT_POWER, power, gain, this );
      break;

    default: assert( 0 );
    }

  }

  virtual bool ready()
  {
    return( player -> stim           ==  STIM_NONE );
  }
};

// ==========================================================================
// Food
// ==========================================================================

struct food_t : public action_t
{
  int type;
  gain_t* gain;

  food_t( player_t* p, const std::string& options_str ) :
    action_t( ACTION_USE, "food", p ), type( FOOD_NONE )
  {
    std::string type_str;

    option_t options[] =
    {
      { "type", OPT_STRING, &type_str },
      { NULL, OPT_UNKNOWN, NULL }
    };
    parse_options( options, options_str );

    trigger_gcd = 0;
    harmful = false;
    for ( int i=0; i < FOOD_MAX; i++ )
    {
      if ( type_str == util_t::food_type_string( i ) )
      {
        type = i;
        break;
      }
    }
    assert( type != FOOD_NONE );
    gain = p -> get_gain( "food" );
  }

  virtual void execute()
  {
    player_t* p = player;
    if ( sim -> log ) log_t::output( sim, "%s uses Food %s", p -> name(), util_t::food_type_string( type ) );
    p -> food = type;
    switch ( type )
    {
/*
      case FOOD_BAKED_ROCKFISH:
      p -> stat_gain( STAT_CRIT_RATING, 90 );
      stamina = 90; p -> stat_gain( STAT_STAMINA, stamina );
      break;
*/
    default: assert( 0 );
    }

  }

  virtual bool ready()
  {
    return( player -> food == FOOD_NONE );
  }
};


// ==========================================================================
// consumable_t::create_action
// ==========================================================================

action_t* consumable_t::create_action( player_t*          p,
                                       const std::string& name,
                                       const std::string& options_str )
{
  if ( name == "stim"                 ) return new                 stim_t( p, options_str );
  if ( name == "food"                 ) return new                 food_t( p, options_str );

  return 0;
}
