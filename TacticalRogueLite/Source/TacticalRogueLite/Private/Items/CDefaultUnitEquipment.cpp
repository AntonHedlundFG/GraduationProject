

#include "Items/CDefaultUnitEquipment.h"

#include "GridContent/CUnit.h"
#include "Items/CInventoryComponent.h"

void UCDefaultUnitEquipment::EquipUnit(ACUnit* unit)
{
	for (const auto Item : DefaultEquipment)
	{
		const bool bEquipped = unit->GetInventoryComp()->TryEquipItem(Item);
		
		if(!bEquipped)
			LOG_WARNING("Invalid Default Item");
	}
}
