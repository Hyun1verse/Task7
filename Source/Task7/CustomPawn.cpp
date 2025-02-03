#include "CustomPawn.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

ACustomPawn::ACustomPawn()
{
    CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
    RootComponent = CapsuleComponent;

    SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
    SkeletalMeshComponent->SetupAttachment(CapsuleComponent);

    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
    SpringArm->SetupAttachment(CapsuleComponent);
    SpringArm->TargetArmLength = 300.0f;
    SpringArm->bUsePawnControlRotation = true;

    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    Camera->SetupAttachment(SpringArm);
    Camera->bUsePawnControlRotation = false;

    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = true;
    bUseControllerRotationRoll = false;
    
    CapsuleComponent->SetSimulatePhysics(false);
    SkeletalMeshComponent->SetSimulatePhysics(false);
}

void ACustomPawn::BeginPlay()
{
    Super::BeginPlay();

    if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
    {
        if (ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer())
        {
            if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
            {
                if (InputMappingContext)
                {
                    Subsystem->AddMappingContext(InputMappingContext, 0);
                }
            }
        }
    }
}

void ACustomPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
        if (MoveForwardAction)
        {
            EnhancedInput->BindAction(MoveForwardAction, ETriggerEvent::Triggered, this, &ACustomPawn::MoveForward);
        }
        if (MoveRightAction)
        {
            EnhancedInput->BindAction(MoveRightAction, ETriggerEvent::Triggered, this, &ACustomPawn::MoveRight);
        }
        if (LookAction)
        {
            EnhancedInput->BindAction(LookAction, ETriggerEvent::Triggered, this, &ACustomPawn::Look);
        }
    }
}

void ACustomPawn::MoveForward(const FInputActionValue& Value)
{
    float InputValue = Value.Get<float>();

    if (Controller && InputValue != 0.0f)
    {
        FRotator ControlRotation = Controller->GetControlRotation();
        FRotator YawRotation(0, ControlRotation.Yaw, 0);
        
        FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
        
        AddActorWorldOffset(ForwardDirection * InputValue * MoveSpeed * GetWorld()->GetDeltaSeconds(), true);
    }
}

void ACustomPawn::MoveRight(const FInputActionValue& Value)
{
    float InputValue = Value.Get<float>();

    if (Controller && InputValue != 0.0f)
    {
        FRotator ControlRotation = Controller->GetControlRotation();
        FRotator YawRotation(0, ControlRotation.Yaw, 0);
        
        FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
        
        AddActorWorldOffset(RightDirection * InputValue * MoveSpeed * GetWorld()->GetDeltaSeconds(), true);
    }
}

void ACustomPawn::Look(const FInputActionValue& Value)
{
    FVector2D LookInput = Value.Get<FVector2D>();

    FRotator NewRotation = GetControlRotation();
    NewRotation.Yaw += LookInput.X * RotationSpeed;
    NewRotation.Pitch = FMath::Clamp(NewRotation.Pitch + LookInput.Y * RotationSpeed, -80.0f, 80.0f);

    GetController()->SetControlRotation(NewRotation);
}
