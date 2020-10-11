// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "NineGridGeneraterComponent.generated.h"

struct Cell_z
{
	AActor* mcell;
	bool beserved;
	bool iscenter;
	Cell_z()
	{
		mcell = nullptr;
		beserved = false;
		iscenter = false;
	}
	Cell_z  operator =(Cell_z c)
	{
		this->mcell = c.mcell;
		this->beserved = c.beserved;
		this->iscenter = c.iscenter;
		return *this;
	}
};
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class NINEGRID_API UNineGridGeneraterComponent : public UActorComponent
{
	GENERATED_BODY()
	FTimerHandle th;
	void timerworker();
	FVector boundsize;
	FVector precenterlocation=FVector(0xffffffff);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnfindnewspawnpoint, const FVector&, oldlocation, const FVector&, newlocation, const FVector&, boundsize, bool, b_isxzplane);
public:
	//FVector location = GetOwner()->GetActorLocation() + (GetOwner()->GetActorForwardVector()* forwardoffset);
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "NineGrid")
    float forwardoffset;
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnfindnewspawnpoint onfindnewspawnpointevent;
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnfindnewspawnpoint onfindnewcenterevent;
	// Sets default values for this component's properties
	UNineGridGeneraterComponent();
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "NineGrid")
	TSubclassOf<class AActor> Cellclass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "NineGrid")
	bool b_xzplane;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	AActor* spawncell(FVector location);

	Cell_z cellzs[9];

	void resetallcell();
	Cell_z* detectedcellbyoffset(FVector detectedlocation);
	Cell_z *findadeserved(FVector center);
	AActor* spawncentercell(FVector location);
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
