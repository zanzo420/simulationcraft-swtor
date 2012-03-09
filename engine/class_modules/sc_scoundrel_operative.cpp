// ==========================================================================
// SimulationCraft for Star Wars: The Old Republic
// http://code.google.com/p/simulationcraft-swtor/
// ==========================================================================

#include "../simulationcraft.hpp"

struct scoundrel_operative_targetdata_t : public targetdata_t
{
  scoundrel_operative_targetdata_t( player_t& source, player_t& target )
    : targetdata_t( source, target )
  {}
};


// ==========================================================================
// Scoundrel / Operative
// ==========================================================================

struct scoundrel_operative_t : public player_t
{
    // Buffs
    struct buffs_t
    {

    } buffs;

    // Gains
    struct gains_t
    {

    } gains;

    // Procs
    struct procs_t
    {

    } procs;

    // RNGs
    struct rngs_t
    {

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
        // Tree 1


        // Tree 2


        // Tree 3

    } talents;

    scoundrel_operative_t( sim_t* sim, player_type pt, const std::string& name, race_type r = RACE_NONE ) :
        player_t( sim, pt == IA_OPERATIVE ? IA_OPERATIVE: S_SCOUNDREL, name, ( r == RACE_NONE ) ? RACE_HUMAN : r )
    {


      primary_attribute   = ATTR_STRENGTH;
      secondary_attribute = ATTR_WILLPOWER;

      create_talents();
      create_options();
    }

    // Character Definition
    virtual targetdata_t* new_targetdata( player_t& target ) // override
    { return new scoundrel_operative_targetdata_t( *this, target ); }

    virtual action_t* create_action( const std::string& name, const std::string& options );
    virtual void      init_talents();
    virtual void      init_base();
    virtual void      init_benefits();
    virtual void      init_buffs();
    virtual void      init_gains();
    virtual void      init_procs();
    virtual void      init_rng();
    virtual void      init_actions();
    virtual int       primary_resource() const;
    virtual int       primary_role() const;
    virtual void      create_talents();

    virtual void init_scaling()
    {
      player_t::init_scaling();
      scales_with[ STAT_FORCE_POWER ] = true;
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

  scoundrel_operative_t* cast() const
  { return static_cast<scoundrel_operative_t*>( player ); }
};

// ==========================================================================
// Scoundrel / Operative Abilities
// ==========================================================================

struct scoundrel_operative_attack_t : public scoundrel_operative_action_t
{
    scoundrel_operative_attack_t( const std::string& n, scoundrel_operative_t* p, school_type s=SCHOOL_KINETIC ) :
      scoundrel_operative_action_t( n, p, melee_policy, RESOURCE_NONE, s )
    {
        may_crit   = true;
    }

};

struct scoundrel_operative_spell_t : public scoundrel_operative_action_t
{
    scoundrel_operative_spell_t( const std::string& n, scoundrel_operative_t* p, school_type s=SCHOOL_KINETIC ) :
      scoundrel_operative_action_t( n, p, force_policy, RESOURCE_NONE, s )
    {
        may_crit   = true;
        tick_may_crit = true;
    }

};

} // ANONYMOUS NAMESPACE ====================================================

// ==========================================================================
// scoundrel_operative Character Definition
// ==========================================================================

// scoundrel_operative_t::create_action ====================================================

action_t* scoundrel_operative_t::create_action( const std::string& name,
                                            const std::string& options_str )
{
    if ( type == IA_OPERATIVE )
    {

    }
    else if ( type == S_SCOUNDREL )
    {

    }

    //if ( name == "apply_charge"           ) return new        apply_charge_t( this, options_str );

    return player_t::create_action( name, options_str );
}

// scoundrel_operative_t::init_talents =====================================================

void scoundrel_operative_t::init_talents()
{
    player_t::init_talents();

    // Darkness|Kinetic Combat
    //talents.thrashing_blades      = find_talent( "Thrashing Blades" );

    // Deception|Infiltration

    // Madness|Balance
}

// scoundrel_operative_t::init_base ========================================================

void scoundrel_operative_t::init_base()
{
    player_t::init_base();

    default_distance = 3;
    distance = default_distance;

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
    // buff_t( player, name, spellname, chance=-1, cd=-1, quiet=false, reverse=false, rng_type=RNG_CYCLIC, activated=true )

    //bool is_juggernaut = ( type == SITH_MARAUDER );



}

// scoundrel_operative_t::init_gains =======================================================

void scoundrel_operative_t::init_gains()
{
    player_t::init_gains();

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
            action_list_str += "stim,type=exotech_resolve";
            action_list_str += "/snapshot_stats";

            switch ( primary_tree() )
            {


            default: break;
            }

            action_list_default = 1;
        }

        // S_SCOUNDREL
        else
        {
            action_list_str += "stim,type=exotech_resolve";
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

// scoundrel_operative_t::primary_resource ==================================================

int scoundrel_operative_t::primary_resource() const
{
    return RESOURCE_FORCE;
}

// scoundrel_operative_t::primary_role ==================================================

int scoundrel_operative_t::primary_role() const
{
    switch ( player_t::primary_role() )
    {
    case ROLE_TANK:
        return ROLE_TANK;
    case ROLE_DPS:
    case ROLE_HYBRID:
        return ROLE_HYBRID;
    default:

        break;
    }

    return ROLE_HYBRID;
}

// scoundrel_operative_t::create_talents ==================================================

void scoundrel_operative_t::create_talents()
{
#if 0
    static const struct
    {
        const char* name;
        int maxpoints;
    } talent_descriptions[] = {
        // Darkness|Kinetic Combat
        { "Thrashing Blades", 2 }, { "Lightning Reflexes", 2 }, { "Charge Mastery", 3 },
        { "Shroud of Darkness", 3 }, { "Lightning Recovery", 2 }, { "Swelling Shadows", 2 }, { "Electric Execution", 3 },
        { "Disjunction", 1 }, { "Energize", 1 }, { "Dark Ward", 1 }, { "Premonition", 2 },
        { "Hollow", 2 }, { "Blood of Sith", 3 },
        { "Electrify", 1 }, { "Eye of the Storm", 1 }, { "Force Pull", 1 }, { "Nerve Wracking", 3 },
        { "Harnessed Darkness", 2 }, { "Mounting Darkness", 3 },
        { "Wither", 1 },
        { 0, 0 },

        // Deception|Infiltration
        { "Insulation", 2 }, { "Duplicity", 3 }, { "Dark Embrace", 2 },
        { "Obfuscation", 3 }, { "Recirculation", 2 }, { "Avoidance", 2 },
        { "Induction", 2 }, { "Surging Charge", 1 }, { "Darkswell", 1 }, { "Deceptive Power", 1 },
        { "Entropic Field", 2 }, { "Saber Conduit", 3 }, { "Fade", 2 }, { "Static Cling", 2 },
        { "Resourcefulness", 2 }, { "Static Charges", 2 }, { "Low Slash", 1 },
        { "Crackling Blasts", 5 },
        { "Voltaic Slash", 1 },
        { 0, 0 },

        // Madness|Balance
        { "Exploitive Strikes", 3 }, { "Sith Defiance", 2 }, { "Crackling Charge", 2 },
        { "Oppressing Force", 2 }, { "Chain Shock", 3 }, { "Parasitism", 2 }, { "Torment", 2 },
        { "Death Field", 1 }, { "Fanaticism", 2 }, { "Claws of Decay", 2 },
        { "Haunted Dreams", 2 }, { "Corrupted Flesh", 2 }, { "Raze", 1 },
        { "Deathmark", 1 }, { "Calculating Mind", 2 }, { "Unearthed Knowledge", 2 },
        { "Creeping Death", 3 }, { "Devour", 2 },
        { "Creeping Terror", 1 },
        { 0, 0 },

    };

    unsigned i = 0;
    for ( unsigned tree = 0; tree < 3; ++tree )
    {
        for(; talent_descriptions[ i ].name != 0; ++i )
        {
            talent_trees[ tree ].push_back( new talent_t( this, talent_descriptions[ i ].name, tree,
                                                          talent_descriptions[ i ].maxpoints ) );
        }
        ++i;
    }
#endif
    player_t::create_talents();
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
