// Fill out your copyright notice in the Description page of Project Settings.


#include "NineGridGeneraterComponent.h"
#include "Engine.h"
// Sets default values for this component's properties
UNineGridGeneraterComponent::UNineGridGeneraterComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	//PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UNineGridGeneraterComponent::BeginPlay()
{
	Super::BeginPlay();

	//UClass* gridclass = LoadClass<AActor>(NULL,TEXT(""));
	FVector vec = GetOwner()->GetActorLocation();
	if (b_xzplane)
	{
		vec.Y -= 100;
		Cell_z tcz;
		tcz.mcell = spawncentercell(vec);
		tcz.beserved = true;
		tcz.iscenter = true;
		cellzs[0] = tcz;
		tcz.mcell = spawncell(vec + FVector(boundsize.X * 2, 0, 0));
		tcz.beserved = true;
		tcz.iscenter = false;
		cellzs[1] = tcz;
		tcz.mcell = spawncell(vec + FVector(-boundsize.X * 2, 0, 0));
		tcz.beserved = true;
		cellzs[2] = tcz;
		tcz.mcell = spawncell(vec + FVector(0, 0, -boundsize.Z * 2));
		tcz.beserved = true;
		cellzs[3] = tcz;
		tcz.mcell = spawncell(vec + FVector(0, 0, boundsize.Z * 2));
		tcz.beserved = true;
		cellzs[4] = tcz;
		tcz.mcell = spawncell(vec + FVector(boundsize.X * 2, 0, boundsize.Z * 2));
		tcz.beserved = true;
		cellzs[5] = tcz;
		tcz.mcell = spawncell(vec + FVector(-boundsize.X * 2, 0, boundsize.Z * 2));
		tcz.beserved = true;
		cellzs[6] = tcz;
		tcz.mcell = spawncell(vec + FVector(-boundsize.X * 2, 0, -boundsize.Z * 2));
		tcz.beserved = true;
		cellzs[7] = tcz;
		tcz.mcell = spawncell(vec + FVector(boundsize.X * 2, 0, -boundsize.Z * 2));
		tcz.beserved = true;
		cellzs[8] = tcz;
	}
	else
	{
		vec.Z -= 100;
		Cell_z tcz;
		tcz.mcell = spawncentercell(vec);
		tcz.beserved = true;
		tcz.iscenter = true;
		cellzs[0] = tcz;
		tcz.mcell = spawncell(vec + FVector(boundsize.X * 2, 0, 0));
		tcz.beserved = true;
		tcz.iscenter = false;
		cellzs[1] = tcz;
		tcz.mcell = spawncell(vec + FVector(-boundsize.X * 2, 0, 0));
		tcz.beserved = true;
		cellzs[2] = tcz;
		tcz.mcell = spawncell(vec + FVector(0, -boundsize.Y * 2, 0));
		tcz.beserved = true;
		cellzs[3] = tcz;
		tcz.mcell = spawncell(vec + FVector(0, boundsize.Y * 2, 0));
		tcz.beserved = true;
		cellzs[4] = tcz;
		tcz.mcell = spawncell(vec + FVector(boundsize.X * 2, boundsize.Y * 2, 0));
		tcz.beserved = true;
		cellzs[5] = tcz;
		tcz.mcell = spawncell(vec + FVector(-boundsize.X * 2, boundsize.Y * 2, 0));
		tcz.beserved = true;
		cellzs[6] = tcz;
		tcz.mcell = spawncell(vec + FVector(-boundsize.X * 2, -boundsize.Y * 2, 0));
		tcz.beserved = true;
		cellzs[7] = tcz;
		tcz.mcell = spawncell(vec + FVector(boundsize.X * 2, -boundsize.Y * 2, 0));
		tcz.beserved = true;
		cellzs[8] = tcz;
	}

	GetWorld()->GetTimerManager().SetTimer(th,this,&UNineGridGeneraterComponent::timerworker,2,true,2);
}
AActor* UNineGridGeneraterComponent::spawncell(FVector location)
{
	FVector Location = location;
	FRotator Rotation = FRotator(0,0,0);
	FActorSpawnParameters ActorSpawnParams;
	ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	AActor* mactor = GetWorld()->SpawnActor<AActor>(Cellclass, Location, Rotation, ActorSpawnParams);
	return mactor;
}
AActor* UNineGridGeneraterComponent::spawncentercell(FVector location)
{
	FVector Location = location;
	FRotator Rotation = FRotator(0, 0, 0);
	FActorSpawnParameters ActorSpawnParams;
	ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	AActor* mactor = GetWorld()->SpawnActor<AActor>(Cellclass, Location, Rotation, ActorSpawnParams);
	FVector original;
	mactor->GetActorBounds(false,original,boundsize);
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, boundsize.ToString().Append(" :boundsize"));

	return mactor;
}
void UNineGridGeneraterComponent::timerworker()
{
		FVector location = GetOwner()->GetActorLocation();

		for (int i = 0; i < 9; i++)
		{
			if (cellzs[i].iscenter)
			{
				continue;
			}
			if (b_xzplane)
			{
				float deltax = cellzs[i].mcell->GetActorLocation().X - location.X;
				if (deltax < 0)
				{
					deltax = -deltax;
				}
				float deltay = cellzs[i].mcell->GetActorLocation().Z - location.Z;
				if (deltay < 0)
				{
					deltay = -deltay;
				}
				if (deltax < (boundsize.X) && deltay < (boundsize.Z))
				{
					GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("recenter+++++++++++++++++++++++++++"));

					resetallcell();
					cellzs[i].iscenter = true;
					FVector centerlocation = cellzs[i].mcell->GetActorLocation();

					FVector up = centerlocation + FVector(boundsize.X * 2, 0, 0);
					Cell_z* cellp = detectedcellbyoffset(up);
					if (cellp)
					{
						cellp->beserved = true;
					}
					else
					{
						Cell_z* fcellp = findadeserved(centerlocation);
						check(fcellp);
						fcellp->mcell->SetActorLocation(up);
					}
					FVector down = centerlocation + FVector(-boundsize.X * 2, 0, 0);
					cellp = detectedcellbyoffset(down);
					if (cellp)
					{
						cellp->beserved = true;
					}
					else
					{
						Cell_z* fcellp = findadeserved(centerlocation);
						check(fcellp);
						fcellp->mcell->SetActorLocation(down);
					}
					FVector left = centerlocation + FVector(0, 0, -boundsize.Z * 2);
					cellp = detectedcellbyoffset(left);
					if (cellp)
					{
						cellp->beserved = true;
					}
					else
					{
						Cell_z* fcellp = findadeserved(centerlocation);
						check(fcellp);
						fcellp->mcell->SetActorLocation(left);
					}
					FVector right = centerlocation + FVector(0, 0, boundsize.Z * 2);
					cellp = detectedcellbyoffset(right);
					if (cellp)
					{
						cellp->beserved = true;
					}
					else
					{
						Cell_z* fcellp = findadeserved(centerlocation);
						check(fcellp);
						fcellp->mcell->SetActorLocation(right);
					}
					FVector up_right = centerlocation + FVector(boundsize.X * 2, 0, boundsize.Z * 2);
					cellp = detectedcellbyoffset(up_right);
					if (cellp)
					{
						cellp->beserved = true;
					}
					else
					{
						Cell_z* fcellp = findadeserved(centerlocation);
						check(fcellp);
						fcellp->mcell->SetActorLocation(up_right);
					}
					FVector right_down = centerlocation + FVector(-boundsize.X * 2, 0, boundsize.Z * 2);
					cellp = detectedcellbyoffset(right_down);
					if (cellp)
					{
						cellp->beserved = true;
					}
					else
					{
						Cell_z* fcellp = findadeserved(centerlocation);
						check(fcellp);
						fcellp->mcell->SetActorLocation(right_down);
					}
					FVector down_left = centerlocation + FVector(-boundsize.X * 2, 0, -boundsize.Z * 2);
					cellp = detectedcellbyoffset(down_left);
					if (cellp)
					{
						cellp->beserved = true;
					}
					else
					{
						Cell_z* fcellp = findadeserved(centerlocation);
						check(fcellp);
						fcellp->mcell->SetActorLocation(down_left);
					}
					FVector left_up = centerlocation + FVector(boundsize.X * 2, 0, -boundsize.Z * 2);
					cellp = detectedcellbyoffset(left_up);
					if (cellp)
					{
						cellp->beserved = true;
					}
					else
					{
						Cell_z* fcellp = findadeserved(centerlocation);
						check(fcellp);
						fcellp->mcell->SetActorLocation(left_up);
					}
				}
			}
			else
			{
				float deltax = cellzs[i].mcell->GetActorLocation().X - location.X;
				if (deltax < 0)
				{
					deltax = -deltax;
				}
				float deltay = cellzs[i].mcell->GetActorLocation().Y - location.Y;
				if (deltay < 0)
				{
					deltay = -deltay;
				}
				if (deltax < (boundsize.X) && deltay < (boundsize.Y))
				{
					GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("recenter+++++++++++++++++++++++++++"));

					resetallcell();
					cellzs[i].iscenter = true;
					FVector centerlocation = cellzs[i].mcell->GetActorLocation();

					FVector up = centerlocation + FVector(boundsize.X * 2, 0, 0);
					Cell_z* cellp = detectedcellbyoffset(up);
					if (cellp)
					{
						cellp->beserved = true;
					}
					else
					{
						Cell_z* fcellp = findadeserved(centerlocation);
						check(fcellp);
						fcellp->mcell->SetActorLocation(up);
					}
					FVector down = centerlocation + FVector(-boundsize.X * 2, 0, 0);
					cellp = detectedcellbyoffset(down);
					if (cellp)
					{
						cellp->beserved = true;
					}
					else
					{
						Cell_z* fcellp = findadeserved(centerlocation);
						check(fcellp);
						fcellp->mcell->SetActorLocation(down);
					}
					FVector left = centerlocation + FVector(0, -boundsize.Y * 2, 0);
					cellp = detectedcellbyoffset(left);
					if (cellp)
					{
						cellp->beserved = true;
					}
					else
					{
						Cell_z* fcellp = findadeserved(centerlocation);
						check(fcellp);
						fcellp->mcell->SetActorLocation(left);
					}
					FVector right = centerlocation + FVector(0, boundsize.Y * 2, 0);
					cellp = detectedcellbyoffset(right);
					if (cellp)
					{
						cellp->beserved = true;
					}
					else
					{
						Cell_z* fcellp = findadeserved(centerlocation);
						check(fcellp);
						fcellp->mcell->SetActorLocation(right);
					}
					FVector up_right = centerlocation + FVector(boundsize.X * 2, boundsize.Y * 2, 0);
					cellp = detectedcellbyoffset(up_right);
					if (cellp)
					{
						cellp->beserved = true;
					}
					else
					{
						Cell_z* fcellp = findadeserved(centerlocation);
						check(fcellp);
						fcellp->mcell->SetActorLocation(up_right);
					}
					FVector right_down = centerlocation + FVector(-boundsize.X * 2, boundsize.Y * 2, 0);
					cellp = detectedcellbyoffset(right_down);
					if (cellp)
					{
						cellp->beserved = true;
					}
					else
					{
						Cell_z* fcellp = findadeserved(centerlocation);
						check(fcellp);
						fcellp->mcell->SetActorLocation(right_down);
					}
					FVector down_left = centerlocation + FVector(-boundsize.X * 2, -boundsize.Y * 2, 0);
					cellp = detectedcellbyoffset(down_left);
					if (cellp)
					{
						cellp->beserved = true;
					}
					else
					{
						Cell_z* fcellp = findadeserved(centerlocation);
						check(fcellp);
						fcellp->mcell->SetActorLocation(down_left);
					}
					FVector left_up = centerlocation + FVector(boundsize.X * 2, -boundsize.Y * 2, 0);
					cellp = detectedcellbyoffset(left_up);
					if (cellp)
					{
						cellp->beserved = true;
					}
					else
					{
						Cell_z* fcellp = findadeserved(centerlocation);
						check(fcellp);
						fcellp->mcell->SetActorLocation(left_up);
					}
				}
			}

		}
}
// Called every frame
void UNineGridGeneraterComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}
void UNineGridGeneraterComponent::resetallcell()
{
	for (int i = 0; i < 9; i++)
	{
		cellzs[i].beserved = false;
		cellzs[i].iscenter = false;
	}
}
Cell_z* UNineGridGeneraterComponent::detectedcellbyoffset(FVector detectedlocation)
{
	for (int i = 0; i < 9; i++)
	{
		float dis = FVector::Distance(cellzs[i].mcell->GetActorLocation(), detectedlocation);
		if (dis < 5)
		{
			return &cellzs[i];
		}
	}
	return NULL;
}
Cell_z* UNineGridGeneraterComponent::findadeserved(FVector center)
{
	if (b_xzplane)
	{
		for (int i = 0; i < 9; i++)
		{
			float deltax = cellzs[i].mcell->GetActorLocation().X - center.X;
			if (deltax < 0)
			{
				deltax = -deltax;
			}
			float deltay = cellzs[i].mcell->GetActorLocation().Z - center.Z;
			if (deltay < 0)
			{
				deltay = -deltay;
			}
			if (deltax > (boundsize.X * 3) || deltay > (boundsize.Z * 3))
			{
				return &cellzs[i];
			}
		}
	}
	else
	{
		for (int i = 0; i < 9; i++)
		{
			float deltax = cellzs[i].mcell->GetActorLocation().X - center.X;
			if (deltax < 0)
			{
				deltax = -deltax;
			}
			float deltay = cellzs[i].mcell->GetActorLocation().Y - center.Y;
			if (deltay < 0)
			{
				deltay = -deltay;
			}
			if (deltax > (boundsize.X * 3) || deltay > (boundsize.Y * 3))
			{
				return &cellzs[i];
			}
		}
	}


	return NULL;
}