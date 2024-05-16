

#include "Items/CDefaultUnitEquipment.h"
#include "GridContent/CUnit.h"
#include "Items/CInventoryComponent.h"
#include "Utility/Logging/CLogManager.h"

void UCDefaultUnitEquipment::EquipUnit(ACUnit* unit)
{
	for (UCItemData* Item : DefaultEquipment)
	{
		if (!Item)
		{
			LOG_WARNING("Null Default Item");
			continue;
		}
		
		UCInventoryComponent* Inventory = unit->GetInventoryComp();
		if (Inventory->CheckValidEquipmentTag(Item->ItemSlot))
		{
			const bool bEquipped = Inventory->TryEquipItem(Item);
			if (!bEquipped)
			{
				LOG_WARNING("DefaultEquipment: Couldn't equip item");
			}
		}
		else
		{
			Inventory->AddItem(Item);
		}
		
	}
}
