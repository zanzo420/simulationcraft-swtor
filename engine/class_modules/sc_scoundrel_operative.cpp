// ==========================================================================
// SimulationCraft for Star Wars: The Old Republic
// http://code.google.com/p/simulationcraft-swtor/
// ==========================================================================

#include "../simulationcraft.hpp"

struct scoundrel_operative_targetdata_t : public targetdata_t
{
  dot_t dot_corrosive_dart;
  dot_t dot_adrenaline_probe;
  dot_t dot_acid_blade_poison;

  scoundrel_operative_targetdata_t( player_t& source, player_t& target ) :
    targetdata_t( source, target ),
    dot_corrosive_dart( "corrosive_dart", &source ),
    dot_adrenaline_probe( "adrenaline_probe", &source ),
    dot_acid_blade_poison( "acid_blade_poison", &source )
  {
    add( dot_corrosive_dart );
    add( dot_adrenaline_probe );
    add( dot_acid_blade_poison );
  }
};


// ==========================================================================
// Scoundrel / Operative
// ==========================================================================

struct scoundrel_operative_t : public player_t
{
  // Buffs
  struct buffs_t
  {
    // buffs from talents
    buff_t* revitalizers;
    buff_t* acid_blade_coating;
    buff_t* acid_blade_arpen;
    //buff_t* advanced_cloaking;

    // core buffs
    buff_t* stealth;
    buff_t* stim_boost;
    buff_t* tactical_advantage;
    //buff_t* cloaking_screen;
  } buffs;

  // Gains
  struct gains_t
  {
    gain_t* low;
    gain_t* med;
    gain_t* high;
    gain_t* adrenaline_probe;
  } gains;

  // Procs
  struct procs_t
  {
  } procs;

  // RNGs
  struct rngs_t
  {
    rng_t* collateral_strike;
  } rngs;

  // Benefits
  struct benefits_t
  {
  } benefits;

  // Cooldowns
  struct cooldowns_t
  {
  } cooldowns;

  // Talents
  struct talents_t
  {
    // Medicine|Sawbones
    // t1
    talent_t* incisive_action;
    talent_t* precision_instruments;
    talent_t* imperial_education;
    // t2
    talent_t* endorphin_rush;
    talent_t* medical_consult;
    talent_t* surical_steadiness;
    talent_t* chem_resistant_inlays;
    // t3
    talent_t* prognosis_critical;
    talent_t* kolto_probe;
    talent_t* sedatives;
    // t4
    talent_t* patient_studies;
    talent_t* medical_engineering;
    talent_t* evasive_imperative;
    // t5
    talent_t* tox_scan;
    talent_t* medical_therapy;
    talent_t* surgical_probe;
    talent_t* surgical_precision;
    // t6
    talent_t* med_shield;
    talent_t* accomplished_doctor;
    // t7
    talent_t* recuperative_nanotech;

    // Concealment|Scrapper
    talent_t* concealed_attacks;
    talent_t* imperial_brew;
    talent_t* survival_training;
    talent_t* infiltrator;
    talent_t* surgical_strikes;
    talent_t* inclement_conditioning;
    talent_t* scouting;
    talent_t* flanking;
    talent_t* laceration;
    talent_t* collateral_strike;
    talent_t* revitalizers;
    talent_t* pin_down;
    talent_t* tactical_opportunity;
    talent_t* energy_screen;
    talent_t* waylay;
    talent_t* culling;
    talent_t* advanced_cloaking;
    talent_t* meticulously_kept_blades;
    talent_t* jarring_strike;
    talent_t* acid_blade;

    // Lethality|Dirty Fighting
    // t1
    talent_t* deadly_directive;
    talent_t* lethality;
    talent_t* razor_edge;
    // t2
    talent_t* slip_away;
    talent_t* flash_powder;
    talent_t* corrosive_microbes;
    talent_t* lethal_injectors;
    // t3
    talent_t* corrosive_grenades;
    talent_t* combat_stims;
    talent_t* cut_down;
    // t4
    talent_t* lethal_purpose;
    talent_t* adhesive_corrosives;
    talent_t* escape_plan;
    talent_t* lethal_dose;
    // t5
    talent_t* cull;
    talent_t* licence_to_kill;
    talent_t* counterstrike;
    // t6
    talent_t* devouring_microbes;
    talent_t* lingering_toxins;
    // t7
    talent_t* weakening_blast;
  } talents;

  action_t* acid_blade_poison;

  scoundrel_operative_t( sim_t* sim, player_type pt, const std::string& name, race_type r = RACE_NONE ) :
    player_t( sim, pt == IA_OPERATIVE ? IA_OPERATIVE : S_SCOUNDREL, name, ( r == RACE_NONE ) ? RACE_HUMAN : r ),
    acid_blade_poison()
  {
    primary_attribute   = ATTR_CUNNING;
    secondary_attribute = ATTR_AIM;

    create_talents();
    create_options();
  }

  // Character Definition
  virtual scoundrel_operative_targetdata_t* new_targetdata( player_t& target ) // override
  { return new scoundrel_operative_targetdata_t( *this, target ); }

  virtual action_t* create_action( const std::string& name, const std::string& options );
  virtual void    init_talents();
  virtual void    init_base();
  virtual void    init_benefits();
  virtual void    init_buffs();
  virtual void    init_gains();
  virtual void    init_procs();
  virtual void    init_rng();
  virtual void    init_actions();
  virtual void    reset();

  virtual double  armor_penetration() const; // override

  std::pair<int,gain_t*> energy_regen_bracket() const;
  virtual double  energy_regen_per_second() const; // override
  virtual void    regen( timespan_t periodicity ); // override

  virtual resource_type primary_resource() const;
  virtual role_type primary_role() const;
          void    create_talents();

  virtual void init_scaling()
  {
    player_t::init_scaling();
    scales_with[ STAT_TECH_POWER ] = true;
  }
};

namespace { // ANONYMOUS NAMESPACE ==========================================

class scoundrel_operative_action_t : public action_t
{
public:
  scoundrel_operative_action_t( const std::string& n, scoundrel_operative_t* player,
                                attack_policy_t policy, resource_type r, school_type s ) :
  action_t( ACTION_ATTACK, n.c_str(), player, policy, r, s )
  {}

  scoundrel_operative_targetdata_t* targetdata() const
  { return static_cast<scoundrel_operative_targetdata_t*>( action_t::targetdata() ); }

  scoundrel_operative_t* p() const
  { return static_cast<scoundrel_operative_t*>( player ); }

  scoundrel_operative_t* cast() const
  { return p(); }
};

// ==========================================================================
// Scoundrel / Operative Abilities
// ==========================================================================

// 4 types of attack: melee, ranged, force, tech. denoted by policy.
// 4 types of damage: kinetic, energy, internal, elemental.
// all combinations possible, but usually patterend.


// action hierarchy
// ----------------
// _action_t - tech_attack  - consume_acid_blade_attack - hidden strike
//                                                      - backstab
//                          - most abilities except...
//           - range_attack - rifle_shot
//                          - overload_shot
//           - acid_blade
//           - adrenaline_probe
//           - stealth


struct scoundrel_operative_tech_attack_t : public scoundrel_operative_action_t
{
  scoundrel_operative_tech_attack_t( const std::string& n, scoundrel_operative_t* p, school_type s=SCHOOL_KINETIC ) :
    scoundrel_operative_action_t( n, p, tech_policy, RESOURCE_ENERGY, s )
  {
    may_crit = true;
  }

  virtual void execute() // override
  {
    scoundrel_operative_action_t::execute();
    p() -> buffs.stealth -> expire();
  }
};

// Consume Acid Blade Poison Attack | ??? ===================================

struct scoundrel_operative_consume_acid_blade_attack_t : public scoundrel_operative_tech_attack_t
{
  scoundrel_operative_consume_acid_blade_attack_t( const std::string& n, scoundrel_operative_t* p, school_type s=SCHOOL_KINETIC ) :
    scoundrel_operative_tech_attack_t( n, p, s )
  {}

  virtual void execute()
  {
    scoundrel_operative_tech_attack_t::execute();

    scoundrel_operative_t* p = cast();
    if ( p -> buffs.acid_blade_coating -> up() )
    {
      p -> buffs.acid_blade_coating -> decrement();
      p -> buffs.acid_blade_arpen -> trigger();

      assert( p -> acid_blade_poison );
      p -> acid_blade_poison -> target = target;
      p -> acid_blade_poison -> execute();
    }
  }
};

struct scoundrel_operative_range_attack_t : public scoundrel_operative_action_t
{
  scoundrel_operative_range_attack_t( const std::string& n, scoundrel_operative_t* p, school_type s=SCHOOL_ENERGY ) :
    scoundrel_operative_action_t( n, p, range_policy, RESOURCE_ENERGY, s )
  {
    may_crit   = true;
  }

  virtual void execute()
  {
    scoundrel_operative_action_t::execute();
    p() -> buffs.stealth -> expire();
  }
};

// Acid Blade | ??? =========================================================

struct acid_blade_t : public scoundrel_operative_action_t
{
  // Acid Blade Poison | ??? ==================================================

  struct acid_blade_poison_t : public scoundrel_operative_tech_attack_t
  {
    acid_blade_poison_t( scoundrel_operative_t* p, const std::string& n ) :
      scoundrel_operative_tech_attack_t( n, p, SCHOOL_INTERNAL )
    {
      td.standardhealthpercentmin = td.standardhealthpercentmax = 0.031;
      td.power_mod = 0.31;
      background = true;
      trigger_gcd = timespan_t::zero();

      num_ticks = 6;
      base_tick_time = from_seconds( 1.0 );
    }
  };

  acid_blade_poison_t* poison;

  acid_blade_t( scoundrel_operative_t* p, const std::string& n, const std::string& options_str ) :
    scoundrel_operative_action_t( n, p, default_policy, RESOURCE_ENERGY, SCHOOL_INTERNAL ),
    poison( new acid_blade_poison_t( p, n + "_poison" ) )
  {
    parse_options( options_str );

    base_cost = 10;
    cooldown -> duration = from_seconds( 2.0 );
    use_off_gcd = true;
    trigger_gcd = timespan_t::zero();

    harmful = false;

    add_child( poison );
  }

  virtual void execute()
  {
    scoundrel_operative_action_t::execute();

    scoundrel_operative_t* p = cast();
    p -> buffs.acid_blade_coating -> trigger();
    p -> acid_blade_poison = poison;
  }
};


// Adrenaline Probe | ??? ===================================================

struct adrenaline_probe_t : public scoundrel_operative_action_t
{
  adrenaline_probe_t( scoundrel_operative_t* p, const std::string& n, const std::string& options_str ) :
    scoundrel_operative_action_t(n, p, default_policy, RESOURCE_ENERGY, SCHOOL_NONE)
  {
    parse_options( options_str );

    cooldown -> duration = from_seconds( 120 );
    use_off_gcd = true;
    trigger_gcd = timespan_t::zero();

    num_ticks = 2;
    base_tick_time = from_seconds( 1.5 );

    harmful = false;
  }

  // the combat log isn't in sync with the game here.
  // the combat log shows after 1.5 seconds a tick of 8 and 34, and then another tick of 8 1.5s later.
  // what happens in game is you instantly get 34, and then two ticks of 8.
  virtual void execute()
  {
    scoundrel_operative_action_t::execute();
    scoundrel_operative_t* p = cast();

    p -> resource_gain( RESOURCE_ENERGY, 34, p -> gains.adrenaline_probe );
  }

  virtual void tick(dot_t* d)
  {
    scoundrel_operative_action_t::tick(d);
    scoundrel_operative_t* p = cast();

    p -> resource_gain( RESOURCE_ENERGY, 8, p -> gains.adrenaline_probe );
  }
};

// Stealth | ??? =========================================================

struct stealth_t : public scoundrel_operative_action_t
{
  stealth_t( scoundrel_operative_t* p, const std::string& n, const std::string& options_str ) :
    scoundrel_operative_action_t( n, p, tech_policy, RESOURCE_ENERGY, SCHOOL_NONE )
  {
    parse_options( options_str );

    // does trigger gcd, but since we only use it out of combat lets make it instant
    trigger_gcd = timespan_t::zero();
    harmful = false;
  }

  virtual bool ready()
  {
    if ( p() -> in_combat )
      return false;

    if ( p() -> buffs.stealth -> check() )
      return false;

    return scoundrel_operative_action_t::ready();
  }

  virtual void execute()
  {
    p() -> buffs.stealth -> trigger();
    scoundrel_operative_action_t::execute();
  }
};

// Shiv | ??? ===============================================================

struct shiv_t : public scoundrel_operative_tech_attack_t
{
  shiv_t( scoundrel_operative_t* p, const std::string& n, const std::string& options_str ) :
    scoundrel_operative_tech_attack_t( n, p )
  {
    rank_level_list = { 2, 5, 8, 11, 14, 19, 29, 38, 50 };

    parse_options( options_str );

    base_cost = 15;
    cooldown -> duration = from_seconds( 6.0 );
    range = 4.0;

    dd.standardhealthpercentmin = 0.148;
    dd.standardhealthpercentmax = 0.188;
    dd.power_mod = 1.68;

    base_multiplier += p->talents.surgical_strikes->rank() * 0.02;
  }

  virtual void execute()
  {
    scoundrel_operative_tech_attack_t::execute();

    // TODO check if granted on misses etc?
    scoundrel_operative_t* p = cast();
    p -> buffs.tactical_advantage -> trigger();

  }
};

// Backstab | ??? ===========================================================

struct backstab_t : public scoundrel_operative_consume_acid_blade_attack_t
{
  backstab_t( scoundrel_operative_t* p, const std::string& n, const std::string& options_str ) :
    scoundrel_operative_consume_acid_blade_attack_t( n, p )
  {
    rank_level_list = { 10, 13, 17, 23, 35, 47, 50 };

    parse_options( options_str );

    base_cost = 10;
    cooldown -> duration = from_seconds( 12.0 );
    range = 4.0;

    dd.standardhealthpercentmin = 0.165;
    dd.standardhealthpercentmax = 0.245;
    dd.power_mod = 2.05;

    base_cost       -= p->talents.flanking->rank() * 5;
    // are these two additive or multiplacitive?
    // Assume additive for now; nearly everything in SWTOR is additive.
    base_multiplier += p->talents.surgical_strikes -> rank() * 0.02
                       +  p->talents.waylay -> rank() * 0.04;
    base_crit       += p -> talents.concealed_attacks->rank() * 0.08;
    crit_bonus      += p->talents.meticulously_kept_blades->rank() * 0.1;
  }
};

// Laceration | ??? =========================================================

struct laceration_t : public scoundrel_operative_tech_attack_t
{
  struct collateral_strike_t : public scoundrel_operative_tech_attack_t
  {
    collateral_strike_t( scoundrel_operative_t* p, const std::string& n, const std::string& options_str ) :
      scoundrel_operative_tech_attack_t( n, p )
    {
      parse_options( options_str );

      dd.standardhealthpercentmin = dd.standardhealthpercentmax = 0.056;
      dd.power_mod = 0.56;

      base_multiplier += p -> talents.culling->rank() * 0.02;

      // FIXME BUG with background set it own't execute itself, butignores cooldown and is never "ready" but only runs on execute
      // but with background set, it runs automatically whenever it can. confused.
      // to explain the problem more clearly.
      // with this structur eof laceration creating a collateral strike action and executing it. If background isn't set then
      // collateral_strike executes itself every 10 seconds, regardless of laceration. I don't understand why.
      // but with background set, the cooldown isn't working and -> remains doesn't work.
      // so currently it executes every time even if it should be on cooldown
      background = true;
      trigger_gcd = timespan_t::zero();
      cooldown -> duration = from_seconds( 10.0 );
    }

    virtual void execute()
    {
      scoundrel_operative_tech_attack_t::execute();

      // if target is poisoned regrant TA
      scoundrel_operative_targetdata_t* td = targetdata();
      if ( td -> dot_acid_blade_poison.ticking || td -> dot_corrosive_dart.ticking )
        p() -> buffs.tactical_advantage -> trigger();
    }
  };

  collateral_strike_t* collateral_strike;

  laceration_t( scoundrel_operative_t* p, const std::string& n, const std::string& options_str ) :
    scoundrel_operative_tech_attack_t( n, p ), collateral_strike( 0 )
  {
    parse_options( options_str );

    base_cost = 10;
    range = 4.0;

    dd.standardhealthpercentmin = 0.14;
    dd.standardhealthpercentmax = 0.22;
    dd.power_mod = 1.8;

    base_multiplier += p -> talents.culling->rank() * 0.02;

    if (  p -> talents.collateral_strike -> rank() )
    {
      collateral_strike = new collateral_strike_t( p, n + "_cs", options_str );
      add_child( collateral_strike );
    }
  }

  virtual bool ready()
  {
    if ( ! p() -> buffs.tactical_advantage -> check() )
      return false;

    return scoundrel_operative_tech_attack_t::ready();
  }

  virtual void execute()
  {
    scoundrel_operative_tech_attack_t::execute();

    scoundrel_operative_t* p = cast();

    // TODO check if a miss etc consumes the TA
    p -> buffs.tactical_advantage -> decrement();

    if ( collateral_strike != nullptr &&
         collateral_strike -> cooldown -> remains() <= timespan_t::zero() &&
         p -> rngs.collateral_strike -> roll ( p -> talents.collateral_strike -> rank() * 0.25 ) )
      collateral_strike -> execute();
  }
};

// Hidden Strike | ??? ======================================================

struct hidden_strike_t : public scoundrel_operative_consume_acid_blade_attack_t
{
  hidden_strike_t( scoundrel_operative_t* p, const std::string& n, const std::string& options_str ) :
    scoundrel_operative_consume_acid_blade_attack_t(n, p)
  {
    rank_level_list = { 36, 50 };

    parse_options( options_str );

    base_cost = 17;
    range = 4.0;
    cooldown -> duration = from_seconds( 7.5 ); // FIXME 7.5 or 8?

    dd.standardhealthpercentmin = 0.218;
    dd.standardhealthpercentmax = 0.278;
    dd.power_mod = 2.48;

    base_crit += p -> talents.concealed_attacks -> rank() * 0.08;
    crit_bonus += p -> talents.meticulously_kept_blades -> rank() * 0.1;
  }

  virtual bool ready()
  {
    scoundrel_operative_t* p = cast();

    if ( p -> buffs.stealth -> up() )
      return scoundrel_operative_action_t::ready();

    return false;
  }

  virtual void execute()
  {
    scoundrel_operative_consume_acid_blade_attack_t::execute();

    scoundrel_operative_t* p = cast();
    p -> buffs.tactical_advantage -> trigger();
  }
  // TODO check for talent and trigger knockdown (jarring strike)
};

// Fragmentation Grenade | ??? ==============================================

struct fragmentation_grenade_t : public scoundrel_operative_tech_attack_t
{
  fragmentation_grenade_t( scoundrel_operative_t* p, const std::string& n, const std::string& options_str ) :
    scoundrel_operative_tech_attack_t(n, p)
  {
    parse_options( 0, options_str );

    base_cost = 20;
    cooldown -> duration = from_seconds( 6.0 );
    range = 30.0;

    dd.standardhealthpercentmin = 0.109;
    dd.standardhealthpercentmax = 0.249;
    dd.power_mod = 1.29;
  }
  // TODO aoe for multiple targets?
};

// Corrosive Dart | ??? =====================================================

struct corrosive_dart_t : public scoundrel_operative_tech_attack_t
{
  corrosive_dart_t( scoundrel_operative_t* p, const std::string& n, const std::string& options_str ) :
    scoundrel_operative_tech_attack_t( n, p, SCHOOL_INTERNAL )
  {
    rank_level_list = { 5, 7, 10, 13, 17, 23, 35, 45, 50 };

    parse_options( options_str );

    base_cost = 20;
    range = 30.0;

    may_crit = false;
    tick_may_crit = true;
    td.standardhealthpercentmin = td.standardhealthpercentmax = 0.04;
    td.power_mod = 0.4;

    num_ticks = 5 + p -> talents.corrosive_microbes -> rank();
    base_tick_time = from_seconds( 3.0 );
  }

  // FIXME: Implement Lethal Injectors
  // along with the rest of the lethality tree
};

// Rifle Shot | ??? =========================================================

struct rifle_shot_t : public scoundrel_operative_range_attack_t
{
  rifle_shot_t* second_strike;

  rifle_shot_t( scoundrel_operative_t* p, const std::string& n, const std::string& options_str,
                bool is_consequent_strike = false ) :
    scoundrel_operative_range_attack_t( n, p ), second_strike( 0 )
  {
    parse_options( 0, options_str );

    base_cost = 0;
    range = 30.0;

    weapon = &( player->main_hand_weapon );
    // FIXME: this isn't working. hitting too weakly
    weapon_multiplier = -1;
    dd.power_mod = 0.5;
    weapon_power_mod = -1;

    // Is a Basic attack
    base_accuracy -= 0.10;

    if ( is_consequent_strike )
    {
      background = true;
      use_off_gcd = true;
      trigger_gcd = timespan_t::zero();
    }
    else
    {
      // TODO does this need options like off the gcd etc?
      second_strike = new rifle_shot_t( p, "rifle_shot_2", options_str, true );
      add_child(second_strike);
    }
  }

  virtual void execute()
  {
    scoundrel_operative_range_attack_t::execute();
    if ( second_strike )
        second_strike->schedule_execute();
  }
};

// Overload Shot | ??? ======================================================

struct overload_shot_t : public scoundrel_operative_range_attack_t
{
  overload_shot_t( scoundrel_operative_t* p, const std::string& n, const std::string& options_str) :
    scoundrel_operative_range_attack_t(n, p)
  {
    rank_level_list = { 8, 12, 16, 22, 31, 40, 50 };

    parse_options( options_str );

    base_cost = 17;
    range = 10.0;

    // FIXME hitting way too weakly
    weapon = &( player->main_hand_weapon );
    dd.standardhealthpercentmin = dd.standardhealthpercentmax = 0.124;
    dd.power_mod = 1.24;

    weapon = &( player->main_hand_weapon );
    weapon_multiplier = -0.17;

    base_multiplier *= 1.0 + 0.15; // skirmisher passive to agents gives 15% boost to overload shot
  }
};

// Stim Boost | ??? =========================================================
// energy and health regen consuming a TA
// waiting on confirmation on implementation

// Cloaking Screen | ??? ====================================================
// "vanish" allows reusing hidden strike.

// Sever Tendon | ??? =======================================================
// does damage, snares and roots talented. mainly for pvp.

// Debilitate | ??? =========================================================
// stuns non bosses and does damage. pvp mainly.


}// ANONYMOUS NAMESPACE ====================================================

// ==========================================================================
// scoundrel_operative Character Definition
// ==========================================================================

// scoundrel_operative_t::create_action ====================================================

action_t* scoundrel_operative_t::create_action( const std::string& name,
                      const std::string& options_str )
{
  if ( type == IA_OPERATIVE )
  {
    if ( name == "stealth" )                return new stealth_t( this, name, options_str );
    if ( name == "shiv" )                   return new shiv_t( this, name, options_str );
    if ( name == "backstab" )               return new backstab_t( this, name, options_str );
    if ( name == "laceration" )             return new laceration_t( this, name, options_str );
    if ( name == "hidden_strike" )          return new hidden_strike_t( this, name, options_str );
    if ( name == "rifle_shot" )             return new rifle_shot_t( this, name, options_str );
    if ( name == "overload_shot" )          return new overload_shot_t( this, name, options_str );
    if ( name == "corrosive_dart" )         return new corrosive_dart_t( this, name, options_str );
    if ( name == "acid_blade" )             return new acid_blade_t( this, name, options_str );
    if ( name == "adrenaline_probe" )       return new adrenaline_probe_t( this, name, options_str );
    if ( name == "fragmentation_grenade" )  return new fragmentation_grenade_t( this, name, options_str );

  }
  else if ( type == S_SCOUNDREL )
  {

  }

  //if ( name == "apply_charge"       ) return new    apply_charge_t( this, options_str );

  return player_t::create_action( name, options_str );
}

// scoundrel_operative_t::init_talents =====================================================

void scoundrel_operative_t::init_talents()
{
  player_t::init_talents();

  // Medicine|Sawbones
  // t1
  talents.incisive_action       = find_talent( "incisive_action" );
  talents.precision_instruments = find_talent( "precision_instruments" );
  talents.imperial_education = find_talent( "imperial_education" );
  // t2
  talents.endorphin_rush            = find_talent( "endorphin_rush" );
  talents.medical_consult           = find_talent( "medical_consult" );
  talents.surical_steadiness        = find_talent( "surical_steadiness" );
  talents.chem_resistant_inlays     = find_talent( "chem_resistant_inlays" );
  // t3
  talents.prognosis_critical        = find_talent( "prognosis_critical" );
  talents.kolto_probe               = find_talent( "kolto_probe" );
  talents.sedatives                 = find_talent( "sedatives" );
  // t4
  talents.patient_studies           = find_talent( "patient_studies" );
  talents.medical_engineering       = find_talent( "medical_engineering" );
  talents.evasive_imperative        = find_talent( "evasive_imperative" );
  // t5
  talents.tox_scan                  = find_talent( "tox_scan" );
  talents.medical_therapy           = find_talent( "medical_therapy" );
  talents.surgical_probe            = find_talent( "surgical_probe" );
  talents.surgical_precision        = find_talent( "surgical_precision" );
  // t6
  talents.med_shield                = find_talent( "med_shield" );
  talents.accomplished_doctor       = find_talent( "accomplished_doctor" );
  // t7
  talents.recuperative_nanotech     = find_talent( "recuperative_nanotech" );

  // Concealment
  talents.concealed_attacks         = find_talent( "Concealed Attacks"        );
  talents.imperial_brew             = find_talent( "Imperial Brew"            );
  talents.survival_training         = find_talent( "Survival Training"        );
  talents.infiltrator               = find_talent( "Infiltrator"              );
  talents.surgical_strikes          = find_talent( "Surgical Strikes"         );
  talents.inclement_conditioning    = find_talent( "Inclement Conditioning"   );
  talents.scouting                  = find_talent( "Scouting"                 );
  talents.flanking                  = find_talent( "Flanking"                 );
  talents.laceration                = find_talent( "Laceration"               );
  talents.collateral_strike         = find_talent( "Collateral Strike"        );
  talents.revitalizers              = find_talent( "Revitalizers"             );
  talents.pin_down                  = find_talent( "Pin Down"                 );
  talents.tactical_opportunity      = find_talent( "Tactical Opportunity"     );
  talents.energy_screen             = find_talent( "Energy Screen"            );
  talents.waylay                    = find_talent( "Waylay"                   );
  talents.culling                   = find_talent( "Culling"                  );
  talents.advanced_cloaking         = find_talent( "Advanced Cloaking"        );
  talents.meticulously_kept_blades  = find_talent( "Meticulously Kept Blades" );
  talents.jarring_strike            = find_talent( "Jarring Strike"           );
  talents.acid_blade                = find_talent( "Acid Blade"               );

    // Lethality|Dirty Fighting
    // t1
  talents.deadly_directive          = find_talent( "deadly_directive" );
  talents.lethality                 = find_talent( "lethality" );
  talents.razor_edge                = find_talent( "razor_edge" );
    // t2
  talents.slip_away                 = find_talent( "slip_away" );
  talents.flash_powder              = find_talent( "flash_powder" );
  talents.corrosive_microbes        = find_talent( "corrosive_microbes" );
  talents.lethal_injectors          = find_talent( "lethal_injectors" );
    // t3
  talents.corrosive_grenades        = find_talent( "corrosive_grenades" );
  talents.combat_stims              = find_talent( "combat_stims" );
  talents.cut_down                  = find_talent( "cut_down" );
    // t4
  talents.lethal_purpose            = find_talent( "lethal_purpose" );
  talents.adhesive_corrosives       = find_talent( "adhesive_corrosives" );
  talents.escape_plan               = find_talent( "escape_plan" );
  talents.lethal_dose               = find_talent( "lethal_dose" );
    // t5
  talents.cull                      = find_talent( "cull" );
  talents.licence_to_kill           = find_talent( "licence_to_kill" );
  talents.counterstrike             = find_talent( "counterstrike" );
    // t6
  talents.devouring_microbes        = find_talent( "devouring_microbes" );
  talents.lingering_toxins          = find_talent( "lingering_toxins" );
    // t7
  talents.weakening_blast           = find_talent( "weakening_blast" );
}

// scoundrel_operative_t::init_base ========================================================

void scoundrel_operative_t::init_base()
{
  player_t::init_base();

  distance = default_distance = 3;

  resource_base[ RESOURCE_ENERGY ] += 100; // TODO 4 piece bonus adds 5 energy
}

// scoundrel_operative_t::init_benefits =======================================================

void scoundrel_operative_t::init_benefits()
{
  player_t::init_benefits();
}

// scoundrel_operative_t::init_buffs =======================================================

void scoundrel_operative_t::init_buffs()
{
  player_t::init_buffs();

  // buff_t( player, name, max_stack, duration, cd=-1, chance=-1, quiet=false, reverse=false, rng_type=RNG_CYCLIC, activated=true )
  // buff_t( player, id, name, chance=-1, cd=-1, quiet=false, reverse=false, rng_type=RNG_CYCLIC, activated=true )

  //bool is_juggernaut = ( type == SITH_MARAUDER );

  buffs.tactical_advantage = new buff_t( this, "tactical_advantage", 2, from_seconds( 10 ) );
  buffs.acid_blade_coating = new buff_t( this, "acid_blade_coating", 1, from_seconds( 20 ) );
  buffs.acid_blade_arpen = new buff_t( this, "acid_blade_arpen", 1, from_seconds( 15 ) );
  buffs.stealth = new buff_t( this, "stealth", 1 );
}

// scoundrel_operative_t::init_gains =======================================================

void scoundrel_operative_t::init_gains()
{
  player_t::init_gains();

  gains.low                     = get_gain( "low"              );
  gains.med                     = get_gain( "med"              );
  gains.high                    = get_gain( "high"             );
  gains.adrenaline_probe        = get_gain( "adrenaline_probe" );

}

// scoundrel_operative_t::init_procs =======================================================

void scoundrel_operative_t::init_procs()
{
  player_t::init_procs();
}

// scoundrel_operative_t::init_rng =========================================================

void scoundrel_operative_t::init_rng()
{
  player_t::init_rng();

  rngs.collateral_strike = get_rng( "collateral_strike" );
}

// scoundrel_operative_t::init_actions =====================================================

void scoundrel_operative_t::init_actions()
{
  //======================================================================================
  //
  //   Please Mirror all changes between Jedi Shadow and Sith Assassin!!!
  //
  //======================================================================================

  if ( action_list_str.empty() )
  {
    if ( type == IA_OPERATIVE )
    {
      action_list_str += "stim,type=exotech_skill";
      action_list_str += "/snapshot_stats";
      action_list_str += "/stealth";
      action_list_str += "/acid_blade,if=!buff.acid_blade_coating.up&buff.stealth.up";
      action_list_str += "/hidden_strike";
      action_list_str += "/corrosive_dart,if=!dot.corrosive_dart.remains&energy>=60";
      action_list_str += "/laceration,if=energy>=70&buff.tactical_advantage.stack>=2";
      action_list_str += "/shiv,if=energy>=75";
      action_list_str += "/acid_blade,if=!buff.acid_blade_coating.up&energy>=75";
      action_list_str += "/backstab,if=energy>=65";
      action_list_str += "/overload_shot,if=energy=100";
      action_list_str += "/adrenaline_probe,if=energy<=60";
      action_list_str += "/rifle_shot";


      switch ( primary_tree() )
      {


      default: break;
      }

      action_list_default = 1;
    }

    // S_SCOUNDREL
    else
    {
      action_list_str += "stim,type=exotech_skill";
      action_list_str += "/snapshot_stats";

      switch ( primary_tree() )
      {

      default: break;
      }

      action_list_default = 1;
    }
  }

  player_t::init_actions();
}

// scoundrel_operative_t::reset =============================================

void scoundrel_operative_t::reset()
{
  acid_blade_poison = 0;
  player_t::reset();
}

// scoundrel_operative_t::primary_resource ==================================

resource_type scoundrel_operative_t::primary_resource() const
{ return RESOURCE_ENERGY; }

// scoundrel_operative_t::primary_role ======================================

role_type scoundrel_operative_t::primary_role() const
{
  switch ( player_t::primary_role() )
  {
  case ROLE_TANK:
    return ROLE_TANK;
  case ROLE_DPS:
  case ROLE_HYBRID:
    return ROLE_HYBRID;
  default:
     if ( primary_tree() == TREE_MEDICINE )
        return ROLE_HEAL;
     if ( primary_tree() == TREE_CONCEALMENT )
       return ROLE_DPS;
     if ( primary_tree() == TREE_LETHALITY )
       return ROLE_DPS;
    break;
  }

  return ROLE_HYBRID;
}

// scoundrel_operative_t::armor_penetration =================================

double scoundrel_operative_t::armor_penetration() const
{
  double arpen = player_t::armor_penetration();

  if ( buffs.acid_blade_arpen -> up() )
    arpen += 0.3;

  return arpen;
}

// scoundrel_operative_t::energy_regen_bracket ==============================

std::pair<int,gain_t*> scoundrel_operative_t::energy_regen_bracket() const
{
  if ( resource_current[ RESOURCE_ENERGY ] < 0.2 * resource_max[ RESOURCE_ENERGY ] )
    return std::make_pair( 2, gains.low );
  else if ( resource_current[ RESOURCE_ENERGY ] < 0.6 * resource_max[ RESOURCE_ENERGY ] )
    return std::make_pair( 3, gains.med );
  else
    return std::make_pair( 5, gains.high );
}

// scoundrel_operative_t::energy_regen_per_second ===========================

double scoundrel_operative_t::energy_regen_per_second() const
{
  double eps = energy_regen_bracket().first;

  if ( buffs.stim_boost -> check() )
    eps += 1;

  return eps;
}

// scoundrel_operative_t::regen =============================================

void scoundrel_operative_t::regen( timespan_t periodicity )
{
  std::pair<int,gain_t*> r = energy_regen_bracket();

  resource_gain( RESOURCE_ENERGY, to_seconds( periodicity ) * r.first, r.second );

  player_t::regen( periodicity );
}

// scoundrel_operative_t::create_talents ====================================

void scoundrel_operative_t::create_talents()
{
  // Medicine
  static const talentinfo_t medicine_tree[] = {
    { "Incisive Action", 2 }, { "Precision Instruments", 2 }, { "Imperial Education", 3 },
    { "Endorphin Rush", 2 }, { "Medical Consult", 3 }, { "Surgical Steadiness", 2 }, { "Chem-resistant Inlays", 2 },
    { "prognosis_critical", 2}, { "kolto_probe", 1}, { "sedatives", 2},
    { "patient_studies", 2}, { "medical_engineering", 3}, { "evasive_imperative", 2},
    { "tox_scan", 1}, { "medical_therapy", 2}, { "surgical_probe", 1}, { "surgical_precision", 1},
    { "med_shield", 2}, { "accomplished_doctor", 3},
    { "recuperative_nanotech", 1},
    // and the rest
  };
  init_talent_tree(IA_OPERATIVE_MEDICINE, medicine_tree );

  // Concealment
  static const talentinfo_t concealment_tree[] = {
    { "Concealed Attacks", 2 }, { "Imperial Brew", 3 }, {"Survival Training", 3},
    { "Infiltrator", 3 }, { "Surgical Strikes", 2 }, { "Inclement Conditioning", 2 }, { "Scouting", 2 },
    { "Flanking", 1 }, { "Laceration", 1 }, {"Collateral Strike", 2 }, { "Revitalizers", 1 },
    { "Pin Down", 2 }, { "Tactical Opportunity", 2 }, { "Energy Screen", 1 },
    { "Waylay", 1 }, { "Culling", 2 }, { "Advanced Cloaking", 2 },
    { "Meticulously Kept Blades", 3 }, { "Jarring Strike", 2 }, { "Acid Blade", 1 },
  };
  init_talent_tree(IA_OPERATIVE_CONCEALMENT , concealment_tree );

  // Lethality
  static const talentinfo_t lethality_tree[] = {
    { "Deadly Directive", 2 }, { "Lethality", 3 }, { "Razor Edge", 2 },
    { "Slip Away", 2 }, { "Flash Powder", 2 }, { "Corrosive Microbes", 2 }, { "Lethal Injectors", 1 },
    { "corrosive_grenades", 1}, { "combat_stims", 2}, { "cut_down", 2},
    { "lethal_purpose", 2}, { "adhesive_corrosives", 2}, { "escape_plan", 2}, { "lethal_dose", 3},
    { "cull", 1}, { "licence_to_kill", 2}, { "counterstrike", 2},
    { "devouring_microbes", 3}, { "lingering_toxins", 2}, { "weakening_blast", 1},
  };
  init_talent_tree(IA_OPERATIVE_LETHALITY, lethality_tree );

}

// ==========================================================================
// PLAYER_T EXTENSIONS
// ==========================================================================

// player_t::create_scoundrel ============================================

player_t* player_t::create_scoundrel( sim_t* sim, const std::string& name, race_type r )
{
  return new scoundrel_operative_t( sim, S_SCOUNDREL, name, r );
}

// player_t::create_operative ==========================================

player_t* player_t::create_operative( sim_t* sim, const std::string& name, race_type r )
{
  return new scoundrel_operative_t( sim, IA_OPERATIVE, name, r );
}

// player_t::scoundrel_operative_init ===========================================

void player_t::scoundrel_operative_init( sim_t* /* sim */ )
{

}

// player_t::scoundrel_operative_combat_begin ===================================

void player_t::scoundrel_operative_combat_begin( sim_t* /* sim */ )
{

}
