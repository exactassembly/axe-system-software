/*
* tps55288-regulator.c -- TI TPS55288
*/

#include <linux/err.h>
#include <linux/i2c.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/of_device.h>
#include <linux/platform_device.h>
#include <linux/regmap.h>
#include <linux/regulator/driver.h>
#include <linux/regulator/machine.h>
#include <linux/regulator/of_regulator.h>
// #include <linux/regulator/tps55288-regulator.h> -- to be made
#include <linux/slab.h>

/* Register definitions */
#define TPS55288_REF_VOLTAGE_LSB_REG    0x00
#define TPS55288_REF_VOLTAGE_MSB_REG    0x01
#define TPS55288_IOUT_LIMIT_REG         0x02
#define TPS55288_VOUT_SR_REG            0x03
#define TPS55288_FEEDBACK_SEL_REG       0x04
#define TPS55288_CDC_REG                0x05
#define TPS55288_MODE_REG               0x06
#define TPS55288_STATUS_REG             0x07

/* REF Voltage */
#define TPS55288_VREF_LSB_MASK 0xff
#define TPS55288_VREF_MSB_MASK 0x03

/* Current Limit */
#define TPS55288_ILIM_EN_MASK 0x80
#define TPS55288_ILIM_SETTING_MASK 0x7F

/* Slew Rate & OCP Delay */
// Slew Rate
#define TPS55288_SLEW_RATE_MASK 0x03
    // Units are mV/us
#define TPS55288_SLEW_RATE_10 0x03
#define TPS55288_SLEW_RATE_5 0x02
#define TPS55288_SLEW_RATE_2_5 0x01
#define TPS55288_SLEW_RATE_1_25 0x00
// Overcurrent protection delay
#define TPS55288_OCP_DELAY_MASK 0x30
#define TPS55288_OCP_DELAY_128US 0x00
#define TPS55288_OCP_DELAY_3MS 0x01
#define TPS55288_OCP_DELAY_6MS 0x02
#define TPS55288_OCP_DELAY_12MS 0x03

/* Feedback Select */
#define TPS55288_FB_SRC_MASK 0x80 // Feedback source (internal vs external)
#define TPS55288_FB_SRC_INTFB 0x00
#define TPS55288_FB_SRC_EXTFB 0x01
//Internal FB ratio (output voltage step)
#define TPS55288_INTFB_MASK 0x03
#define TPS55288_INTFB_20MV 0x03
#define TPS55288_INTFB_15MV 0x02
#define TPS55288_INTFB_10MV 0x01
#define TPS55288_INTFB_5MV 0x00

/* CDC and Protection Masks */
#define TPS55288_CDC_OPT_MASK 0x08
#define TPS55288_CDC_MASK 0x07

#define TPS55288_SC_MASK 0x80
#define TPS55288_OCP_MASK 0x40
#define TPS55288_OVP_MASK 0x20

/* Mode */
#define TPS55288_OUTPUT_EN_MASK 0x80
#define TPS55288_FSWDBL_MASK 0x40
#define TPS55288_HUCCUP_MASK 0x20
#define TPS55288_DISCHG_MASK 0x10
#define TPS55288_VCC_MASK 0x08
#define TPS55288_I2CADD_MASK 0x04
#define TPS55288_PFM_MASK 0x02
#define TPS55288_MODE_MASK 0x01

/* Status */
// protection masks (overvoltage, overcurrent, shortcircuit) are defined in CDC and Protection Masks
#define TPS55288_STATUS_MASK 0x03
#define TPS55288_STATUS_BOOST 0x00
#define TPS55288_STATUS_BUCK 0x01
#define TPS55288_STATUS_BUCKBOOST 0x02


struct tps55288_regulator_platform_data {
    struct regulator_init_data *reg_init_data;
};

struct tps55288_chip {
    struct device *dev;
    struct regulator_desc desc;
    struct regulator_dev *rdev;
    struct regmap *regmap;
};

static int tps55288_dcdc_get_voltage_sel(struct regulator_dev *dev) {
    struct tps55288_chip *tps = rdev_get_drvdata(dev);
    int vsel;
    unsigned int data_lsb;
    unsigned int data_msb;
    int ret;

    ret = regmap_read(tps->regmap, TPS55288_REF_VOLTAGE_LSB_REG, &data_lsb);
    if(ret < 0) {
        dev_err(tps->dev, "%s(): register %d read failed with err %d\n", __func__, TPS55288_REF_VOLTAGE_LSB_REG, ret);
        return ret;
    }

    vsel = (int)data_lsb;
    
    ret = regmap_read(tps->regmap, TPS55288_REF_VOLTAGE_MSB_REG, &data_msb);
    if(ret < 0) {
        dev_err(tps->dev, "%s(): register %d read failed with err %d\n", __func__, TPS55288_REF_VOLTAGE_MSB_REG, ret);
        return ret;
    }

    vsel |= ((int)data_msb & TPS55288_VREF_MSB_MASK) << 8;
    return vsel;
}

static int tps55288_dcdc_set_voltage_sel(struct regulator_dev *dev, unsigned selector) {
    struct tps55288_chip *tps = rdev_get_drvdata(dev);
    int ret;
    
    ret = regmap_update_bits(tps->regmap, TPS55288_REF_VOLTAGE_MSB_REG, TPS55288_VREF_MSB_MASK, (selector >> 8) & TPS55288_VREF_MSB_MASK);
    if(ret < 0) {
        dev_err(tps->dev, "%s(): register %d update failed with err %d\n", __func__, TPS55288_REF_VOLTAGE_MSB_REG, ret);
        return ret;
    }
    
    ret = regmap_write(tps->regmap, TPS55288_REF_VOLTAGE_LSB_REG, selector & TPS55288_VREF_LSB_MASK);
    if(ret < 0) {
        dev_err(tps->dev, "%s(): register %d update failed with err %d\n", __func__, TPS55288_REF_VOLTAGE_LSB_REG, ret);
        return ret;
    }
    

    return 0;
}

static int tps55288_enable(struct regulator_dev *dev) {
    struct tps55288_chip *tps = rdev_get_drvdata(dev);
    int ret;

    ret = regmap_update_bits(tps->regmap, TPS55288_MODE_REG, TPS55288_OUTPUT_EN_MASK, 1);
    if(ret < 0) {
        dev_err(tps->dev, "%s(): register %d update failed with err %d\n", __func__, TPS55288_MODE_REG, ret);
        return ret;
    }

    return ret;
}

static int tps55288_disable(struct regulator_dev *dev) {
    struct tps55288_chip *tps = rdev_get_drvdata(dev);
    int ret;

    ret = regmap_update_bits(tps->regmap, TPS55288_MODE_REG, TPS55288_OUTPUT_EN_MASK, 0);
    if(ret < 0) {
        dev_err(tps->dev, "%s(): register %d update failed with err %d\n", __func__, TPS55288_MODE_REG, ret);
        return ret;
    }

    return ret;
}

static int tps55288_is_enabled(struct regulator_dev *dev) {
    struct tps55288_chip *tps = rdev_get_drvdata(dev);
    int data;
    int ret;

    ret = regmap_read(tps->regmap, TPS55288_MODE_REG, &data);
    if(ret < 0) {
        dev_err(tps->dev, "%s(): register %d read failed with err %d\n", __func__, TPS55288_MODE_REG, ret);
        return ret;
    }

    return (data & TPS55288_OUTPUT_EN_MASK) >> 7;
}

static const struct regulator_ops tps55288_dcdc_ops = {
    .get_voltage_sel    = tps55288_dcdc_get_voltage_sel,
    .set_voltage_sel    = tps55288_dcdc_set_voltage_sel,
    .list_voltage       = regulator_list_voltage_linear,
    .map_voltage        = regulator_map_voltage_linear,
    .enable             = tps55288_enable,
    .disable            = tps55288_disable,
    .is_enabled         = tps55288_is_enabled,
};

static int tps55288_init_dcdc(struct tps55288_chip* tps, struct tps55288_regulator_platform_data *pdata) {
    int ret;


    /* set all registers to desired default settings */
    // disable protections for now
    ret = regmap_update_bits(tps->regmap, TPS55288_VOUT_SR_REG, TPS55288_SC_MASK | TPS55288_OCP_MASK | TPS55288_OVP_MASK, 0x00);
    if(ret < 0) {
        dev_err(tps->dev, "%s(): register %d update failed with err %d\n", __func__, TPS55288_VOUT_SR_REG, ret);
        return ret;
    }


    // disable output
    ret = regmap_update_bits(tps->regmap, TPS55288_MODE_REG, TPS55288_OUTPUT_EN_MASK, 0x00);
    if(ret < 0) {
        dev_err(tps->dev, "%s(): register %d update failed with err %d\n", __func__, TPS55288_MODE_REG, ret);
        return ret;
    }

    // set fbsel to intfb and intfb ratio to 20mV
    ret = regmap_write(tps->regmap, TPS55288_FEEDBACK_SEL_REG, (TPS55288_FB_SRC_MASK & (TPS55288_FB_SRC_INTFB << 7)) | (TPS55288_INTFB_MASK & TPS55288_INTFB_20MV));
    if(ret < 0) {
        dev_err(tps->dev, "%s(): register %d write failed with err %d\n", __func__, TPS55288_FEEDBACK_SEL_REG, ret);
        return ret;
    }    
    
    // Set vref to zero
    ret = regmap_update_bits(tps->regmap, TPS55288_REF_VOLTAGE_MSB_REG, TPS55288_VREF_MSB_MASK, 0x00);
    if(ret < 0) {
        dev_err(tps->dev, "%s(): register %d update failed with err %d\n", __func__, TPS55288_REF_VOLTAGE_MSB_REG, ret);
        return ret;
    }

    ret = regmap_write(tps->regmap, TPS55288_REF_VOLTAGE_LSB_REG, 0x00);
    if(ret < 0) {
        dev_err(tps->dev, "%s(): register %d write failed with err %d\n", __func__, TPS55288_REF_VOLTAGE_LSB_REG, ret);
        return ret;
    }

    return ret;
}

static const struct regmap_config tps55288_regmap_config = {
    .reg_bits               = 8,
    .val_bits               = 8,
    .max_register           = TPS55288_STATUS_REG,
    .cache_type             = REGCACHE_RBTREE,
};

static struct tps55288_regulator_platform_data *of_get_tps55288_platform_data(struct device *dev, const struct regulator_desc *desc) {
    struct tps55288_regulator_platform_data *pdata;
    struct device_node *np = dev->of_node;

    pdata = devm_kzalloc(dev, sizeof(*pdata), GFP_KERNEL);
    if(!pdata) {
        return NULL;
    }

    pdata->reg_init_data = of_get_regulator_init_data(dev, dev->of_node, desc);
    if(!pdata->reg_init_data) {
        dev_err(dev, "Not able to get OF regulator init data\n");
        return NULL;
    }

    // If you want options on the dt-node this is how...
    /*
    pdata->attr = of_property_read_bool/int(np, "property-name");
    if(of_find_property(np, "property-name", NULL)) {
        pdata->attr = val;
    }
    */

    return pdata;
}

#if defined(CONFIG_OF)
static const struct of_device_id tps55288_of_match[] = {
    { .compatible = "ti,tps55288",},
    {},
};
MODULE_DEVICE_TABLE(of, tps55288_of_match);
#endif


static int tps55288_probe(struct i2c_client *client, const struct i2c_device_id *id) {
    struct tps55288_regulator_platform_data *pdata;
    struct regulator_dev *rdev;
    struct tps55288_chip *tps;
    int ret;
    struct regulator_config config = {};

    if(client->dev.of_node) {
        const struct of_device_id *match;
        match = of_match_device(of_match_ptr(tps55288_of_match), &client->dev);

        if(!match) {
            dev_err(&client->dev, "Error: No device match found\n");
            return -ENODEV;
        }
    }

    tps = devm_kzalloc(&client->dev, sizeof(*tps), GFP_KERNEL);
    if(!tps) {
        return -ENOMEM;
    }

    tps->dev = &client->dev;
    tps->desc.name = client->name;
    tps->desc.id = 0;
    // add other desc stuff here
    tps->desc.min_uV = 800000; // 0.8 V
    //tps->desc.max_uV = 20000000; // 20 V
    tps->desc.uV_step = 20017; // 20 mV
    tps->desc.n_voltages = 1023;

    tps->desc.ops = &tps55288_dcdc_ops;
    tps->desc.type = REGULATOR_VOLTAGE | REGULATOR_CURRENT;
    tps->desc.owner = THIS_MODULE;

    pdata = dev_get_platdata(&client->dev);
    if(!pdata && client->dev.of_node) {
        pdata = of_get_tps55288_platform_data(&client->dev, &tps->desc);
    }
    if(!pdata) {
        dev_err(&client->dev, "No Platform data\n");
        return -EINVAL;
    }

    tps->dev = &client->dev;
    tps->regmap = devm_regmap_init_i2c(client, &tps55288_regmap_config);
    if(IS_ERR(tps->regmap)) {
        ret = PTR_ERR(tps->regmap);
        dev_err(&client->dev, "%s(): regmap allocation failed with err %d\n", __func__, ret);
        return ret;
    }
    i2c_set_clientdata(client, tps);

    ret = tps55288_init_dcdc(tps, pdata);
    if(ret < 0) {
        dev_err(tps->dev, "%s(): Init failed with err = %d\n", __func__, ret);
        return ret;
    }

    config.dev = &client->dev;
    config.init_data = pdata->reg_init_data;
    config.driver_data = tps;
    config.of_node = client->dev.of_node;

    // Register the regulator
    rdev = devm_regulator_register(&client->dev, &tps->desc, &config);
    if(IS_ERR(rdev)) {
        dev_err(tps->dev, "%s(): regulator register failed with err %s\n", __func__, id->name);
        return PTR_ERR(rdev);
    }

    tps->rdev = rdev;
    return 0;
}

static const struct i2c_device_id tps55288_id[] = {
    {.name = "tps55288",},
    {},
};

MODULE_DEVICE_TABLE(i2c, tps55288_id);

static struct i2c_driver tps55288_i2c_driver = {
    .driver = {
        .name = "tps55288",
        .of_match_table = of_match_ptr(tps55288_of_match),
    },
    .probe = tps55288_probe,
    .id_table = tps55288_id,
};

static int __init tps55288_init(void) {
    return i2c_add_driver(&tps55288_i2c_driver);
}
subsys_initcall(tps55288_init);

static void __exit tps55288_cleanup(void) {
    i2c_del_driver(&tps55288_i2c_driver);
}
module_exit(tps55288_cleanup);


MODULE_LICENSE("GPL");