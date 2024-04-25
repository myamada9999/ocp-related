#include "qemu/osdep.h"
#include "hw/pci/pci.h"
#include "qapi/error.h"
#include "qapi/visitor.h"
#include "qemu/module.h"

#define TYPE_OCP_NVME "ocp-nvme"
#define OCP_NVME(obj) OBJECT_CHECK(OCPNVMeState, (obj), TYPE_OCP_NVME)

typedef struct {
    PCIDevice parent_obj;
    MemoryRegion mmio;
    /* Add other device specific state variables here */
} OCPNVMeState;

static void ocp_nvme_realize(PCIDevice *pci_dev, Error **errp)
{
    OCPNVMeState *s = OCP_NVME(pci_dev);

    /* Initialize memory regions, device state, etc. */
    memory_region_init_io(&s->mmio, OBJECT(s), &ocp_nvme_ops, s, "ocp-nvme-mmio", 0x1000);
    pci_register_bar(pci_dev, 0, PCI_BASE_ADDRESS_SPACE_MEMORY, &s->mmio);
    
    /* More initialization as needed */
}

static void ocp_nvme_uninit(PCIDevice *pci_dev)
{
    /* Cleanup any allocated resources */
}

static void ocp_nvme_class_init(ObjectClass *klass, void *data)
{
    DeviceClass *dc = DEVICE_CLASS(klass);
    PCIDeviceClass *k = PCI_DEVICE_CLASS(klass);
    
    k->realize = ocp_nvme_realize;
    k->exit = ocp_nvme_uninit;
    k->vendor_id = 0x1af4; /* Vendor ID */
    k->device_id = 0x1001; /* Device ID */
    k->class_id = PCI_CLASS_STORAGE_EXPRESS;
    dc->desc = "OCP NVMe Device";
}

static const TypeInfo ocp_nvme_info = {
    .name          = TYPE_OCP_NVME,
    .parent        = TYPE_PCI_DEVICE,
    .instance_size = sizeof(OCPNVMeState),
    .class_init    = ocp_nvme_class_init,
};

static void ocp_nvme_register_types(void)
{
    type_register_static(&ocp_nvme_info);
}

type_init(ocp_nvme_register_types)
