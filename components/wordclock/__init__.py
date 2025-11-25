import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import light, time, select, switch, number
from esphome.const import CONF_ID

DEPENDENCIES = ['light', 'time']
AUTO_LOAD = ['select', 'switch', 'number']

wordclock_ns = cg.esphome_ns.namespace('wordclock')
Wordclock = wordclock_ns.class_('Wordclock', cg.Component)

CONF_LIGHT_ID = 'light_id'
CONF_TIME_ID = 'time_id'
CONF_TIMEMODUS_ID = 'timemodus_id'
CONF_SHOWDATE_ID = 'showdate_id'
CONF_COLORCYCLE_ID = 'colorcycle_id'
CONF_CCSPEED_ID = 'ccspeed_id'

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(Wordclock),
    cv.Required(CONF_LIGHT_ID): cv.use_id(light.LightState),
    cv.Required(CONF_TIME_ID): cv.use_id(time.RealTimeClock),
    cv.Optional(CONF_TIMEMODUS_ID): cv.use_id(select.Select),
    cv.Optional(CONF_SHOWDATE_ID): cv.use_id(switch.Switch),
    cv.Optional(CONF_COLORCYCLE_ID): cv.use_id(switch.Switch),
    cv.Optional(CONF_CCSPEED_ID): cv.use_id(number.Number),
}).extend(cv.COMPONENT_SCHEMA)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)

    light_var = await cg.get_variable(config[CONF_LIGHT_ID])
    cg.add(var.set_light(light_var))

    time_var = await cg.get_variable(config[CONF_TIME_ID])
    cg.add(var.set_time(time_var))

    if CONF_TIMEMODUS_ID in config:
        timemodus_var = await cg.get_variable(config[CONF_TIMEMODUS_ID])
        cg.add(var.set_timemodus(timemodus_var))

    if CONF_SHOWDATE_ID in config:
        showdate_var = await cg.get_variable(config[CONF_SHOWDATE_ID])
        cg.add(var.set_showdate(showdate_var))

    if CONF_COLORCYCLE_ID in config:
        colorcycle_var = await cg.get_variable(config[CONF_COLORCYCLE_ID])
        cg.add(var.set_colorcycle(colorcycle_var))

    if CONF_CCSPEED_ID in config:
        ccspeed_var = await cg.get_variable(config[CONF_CCSPEED_ID])
        cg.add(var.set_ccspeed(ccspeed_var))